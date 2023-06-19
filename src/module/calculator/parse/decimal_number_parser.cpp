#include "module/calculator/parse/decimal_number_parser.h"
#include <zaf/base/container/utility/contain.h>

using namespace std::string_literals;

namespace ra::mod::calculator {

DecimalNumberParser* DecimalNumberParser::Instance() {

    static DecimalNumberParser instance;
    return &instance;
}


ParseStatus DecimalNumberParser::Parse(ParseContext& context, ParseResult& parse_result) {

    auto reader = context.BeginRead();

    std::wstring integer_part;
    std::wstring float_part;
    auto parse_status = ParseNumberParts(reader, integer_part, float_part);
    if (parse_status != ParseStatus::Ok) {
        return parse_status;
    }

    NumberUnit unit{};
    ParseUnit(reader, unit);

    auto operand_node = CreateOperandNode(integer_part, float_part, unit);
    parse_result.AddOperand(operand_node);
    return ParseStatus::Ok;
}


ParseStatus DecimalNumberParser::ParseNumberParts(
    ParseReader& reader,
    std::wstring& integer_part,
    std::wstring& float_part) {

    static const std::wstring digits = L"0123456789"s;

    bool is_parsing_float_part{};

    do {

        auto ch = reader.GetChar();
        if (ch == L'.') {

            if (is_parsing_float_part) {
                break;
            }

            is_parsing_float_part = true;
        }
        else if (zaf::Contain(digits, ch)) {

            if (is_parsing_float_part) {
                float_part.append(1, ch);
            }
            else {
                integer_part.append(1, ch);
            }
        }
        else {
            break;
        }
    }
    while (reader.Forward());

    if (integer_part.empty() && float_part.empty()) {

        if (is_parsing_float_part) {
            return ParseStatus::Error;
        }
        else {
            return ParseStatus::Mismatched;
        }
    }

    return ParseStatus::Ok;
}


void DecimalNumberParser::ParseUnit(ParseReader& reader, NumberUnit& unit) {

    auto number_unit = GetNumberUnitByChar(reader.GetChar());
    if (number_unit) {

        unit = *number_unit;
        reader.Forward();
    }
    else {

        unit = NumberUnit::Normal;
    }
}


std::shared_ptr<OperandNode> DecimalNumberParser::CreateOperandNode(
    const std::wstring& integer_part,
    const std::wstring& float_part,
    NumberUnit unit) {

    auto operand_node = std::make_shared<OperandNode>();
    operand_node->base = 10;
    operand_node->unit = unit;

    if (integer_part.empty() && !float_part.empty()) {
        operand_node->text = L"0." + float_part;
    }
    else if (!integer_part.empty() && float_part.empty()) {
        operand_node->text = integer_part;
    }
    else {
        operand_node->text = integer_part + L'.' + float_part;
    }

    return operand_node;
}

}