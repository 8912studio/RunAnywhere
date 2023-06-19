#include "module/tool/base64/base64_command_parsing.h"
#include <zaf/base/container/utility/range.h>

namespace ra::mod::tool::base64 {
namespace {

bool TryToParseSwitch(
    std::wstring_view argument,
    Base64CommandParseResult& parse_result) {

    if (argument.empty()) {
        return false;
    }

    if (argument.front() != L'/') {
        return false;
    }

    auto value = argument.substr(1);
    if (value == L"e") {
        parse_result.operation = base64::Base64Operation::Encode;
        return true;
    }

    if (value == L"d") {
        parse_result.operation = base64::Base64Operation::Decode;
        return true;
    }

    return false;
}

}


Base64CommandParseResult Parse(const utility::CommandLine& command_line) {

    Base64CommandParseResult result;
    for (const auto& each_argument : command_line.Arguments()) {

        if (each_argument.empty()) {
            continue;
        }

        if (TryToParseSwitch(each_argument, result)) {
            continue;
        }

        result.input_text = each_argument;
    }
    return result;
}

}