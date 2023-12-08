#include "module/tool/json/json_command_parsing.h"
#include <sstream>
#include <boost/json.hpp>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include "module/tool/json/json_formatter.h"

namespace ra::mod::tool::json {

JSONCommandParseResult ParseJSONCommand(const utility::CommandLine& command_line) {

    const auto& arguments = command_line.Arguments();
    if (arguments.empty()) {
        return JSONCommandParseResult{ 
            zaf::make_error_code(zaf::BasicErrc::InvalidValue),
            0, 
            {} 
        };
    }

    const auto& original_text = arguments.front().Content();

    boost::json::parser parser;
    std::error_code error;
    auto parsed_length = parser.write_some(zaf::ToUTF8String(original_text), error);

    if (error) {
        return JSONCommandParseResult{ error, parsed_length, original_text };
    }

    return JSONCommandParseResult{ JSONFormatter{}.Format(parser.release()) };
}

}