#include "module/calculator/parse/octal_number_parser.h"
#include <zaf/base/container/utility/contain.h>

using namespace std::string_literals;

namespace ra::mod::calculator {

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

    auto reader = context.BeginRead();

    //Check prefix.
    auto ch = reader.GetChar();
    if (ch != L'0') {
        return ParseStatus::Mismatched;
    }

    reader.Forward();

    static const std::wstring digits = L"01234567"s;
    
    bool has_syntax_error{};
    do {

        ch = reader.GetChar();

        if (zaf::Contain(digits, ch)) {
            number.append(1, ch);
        }
        else if (ch == L'8' || ch == L'9') {
            has_syntax_error = true;
            break;
        }
        else {
            break;
        }
    }
    while (reader.Forward());

    if (number.empty() && !has_syntax_error) {
        reader.Discard();
        return ParseStatus::Mismatched;
    }

    if (has_syntax_error) {
        return ParseStatus::Error;
    }

    return ParseStatus::Ok;
}

}