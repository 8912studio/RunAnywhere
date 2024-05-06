#include "module/tool/json/json_command_parsing.h"
#include <sstream>
#include <boost/json.hpp>
#include <zaf/base/string/encoding_conversion.h>
#include "module/tool/json/json_formatter.h"

namespace ra::mod::tool::json {

JSONCommandParseResult ParseJSONCommand(const utility::CommandLine& command_line) {

    const auto& arguments = command_line.Arguments();
    if (arguments.empty()) {
        return JSONCommandParseResult{
            std::make_error_code(std::errc::invalid_argument),
            0,
            {}
        };
    }

    const auto& original_text = arguments.front().Content();
    auto utf8_text = zaf::ToUTF8String(original_text);

    boost::json::parser parser;
    std::error_code error;
    auto parsed_length = parser.write_some(utf8_text, error);

    if (error) {
        return JSONCommandParseResult{ error, parsed_length, original_text };
    }

    if (parsed_length != utf8_text.length()) {
        return JSONCommandParseResult{ 
            make_error_code(boost::json::error::extra_data), 
            parsed_length, 
            original_text 
        };
    }

    return JSONCommandParseResult{ JSONFormatter{}.Format(parser.release()) };
}

}