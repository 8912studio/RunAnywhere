#include "module/tool/json/json_command_parsing.h"
#include <sstream>
#include <boost/json.hpp>
#include <zaf/base/string/encoding_conversion.h>
#include "module/tool/json/json_formatting.h"

namespace ra::mod::tool::json {

JSONCommandParseResult ParseJSONCommand(const utility::CommandLine& command_line) {

    const auto& arguments = command_line.Arguments();
    if (arguments.empty()) {
        return {};
    }

    try {

        auto root = boost::json::parse(zaf::ToUTF8String(arguments.front().Content()));

        JSONCommandParseResult result;
        result.formatted_result = zaf::FromUTF8String(FormatJSONValue(root));
        return result;
    }
    catch (const boost::system::system_error&) {
        return {};
    }
}

}