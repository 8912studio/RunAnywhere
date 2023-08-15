#include "module/tool/base64/base64_command_parsing.h"

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
    }
    else if (value == L"d") {
        parse_result.operation = base64::Base64Operation::Decode;
    }
    else if (value == L"u8") {
        parse_result.encoding = TextEncoding::UTF8;
    }
    else if (value == L"u16") {
        parse_result.encoding = TextEncoding::UTF16;
    }
    else if (value == L"h") {
        parse_result.use_hex = true;
    }
    return true;
}

}


Base64CommandParseResult Parse(const utility::CommandLine& command_line) {

    Base64CommandParseResult result;
    for (const auto& each_argument : command_line.Arguments()) {

        if (each_argument.Type() == utility::CommandLinePieceType::NormalText) {
            if (TryToParseSwitch(each_argument.Content(), result)) {
                continue;
            }
        }

        result.input_text = each_argument.Content();
    }
    return result;
}

}