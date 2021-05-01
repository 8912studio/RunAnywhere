#include "module/calculator/parse/octal_number_parser.h"
#include <zaf/base/container/utility/contain.h>

using namespace std::string_literals;

namespace ra::module::calculator {

OctalNumberParser* OctalNumberParser::Instance() {

    static OctalNumberParser instance;
    return &instance;
}


ParseStatus OctalNumberParser::Parse(ParseContext& context, ParseResult& parse_result) {

    std::wstring number;
    auto parse_status = ParseNumber(context, number);
    if (parse_status != ParseStatus::Ok) {
        return parse_status;
    }

    auto operand_node = std::make_shared<OperandNode>();
    operand_node->base = 8;
    operand_node->text = number;
    parse_result.AddOperand(operand_node);
    return ParseStatus::Ok;
}


ParseStatus OctalNumberParser::ParseNumber(ParseContext& context, std::wstring& number) {

    if (context.GetCurrentChar() != L'0') {
        return ParseStatus::Mismatched;
    }

    if (!context.Forward()) {
        return ParseStatus::Mismatched;
    }

    static const std::wstring digits = L"01234567"s;

    do {

        wchar_t current_char = context.GetCurrentChar();
        if (zaf::Contain(digits, current_char)) {
            number.append(1, current_char);
        }
        else if (current_char == L'8' || current_char == L'9') {
            return ParseStatus::Error;
        }
        else {
            break;
        }
    }
    while (context.Forward());

    if (number.empty()) {

        context.Backward();
        return ParseStatus::Mismatched;
    }

    return ParseStatus::Ok;
}

}