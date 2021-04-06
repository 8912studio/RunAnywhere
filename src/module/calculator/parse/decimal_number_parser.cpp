#include "module/calculator/parse/decimal_number_parser.h"
#include <zaf/base/container/utility/contain.h>

using namespace std::string_literals;

namespace calculator {

DecimalNumberParser* DecimalNumberParser::Instance() {

    static DecimalNumberParser instance;
    return &instance;
}


ParseStatus DecimalNumberParser::Parse(ParseContext& context, ParseResult& parse_result) {

    std::wstring integer_part;
    std::wstring float_part;
    auto parse_status = ParseNumberParts(context, integer_part, float_part);
    if (parse_status != ParseStatus::Ok) {
        return parse_status;
    }

    NumberUnit unit{};
    parse_status = ParseUnit(context, unit);
    if (parse_status != ParseStatus::Ok) {
        return parse_status;
    }

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

    parse_result.AddOperand(operand_node);
    return ParseStatus::Ok;
}


ParseStatus DecimalNumberParser::ParseNumberParts(
    ParseContext& context,
    std::wstring& integer_part,
    std::wstring& float_part) {

    static const std::wstring digits = L"0123456789"s;

    bool is_parsing_float_part{};
    do {

        wchar_t current_char = context.GetCurrentChar();
        if (current_char == L'.') {

            if (is_parsing_float_part) {
                return ParseStatus::Error;
            }

            is_parsing_float_part = true;
        }
        else if (zaf::Contain(digits, current_char)) {

            if (is_parsing_float_part) {
                float_part.append(1, current_char);
            }
            else {
                integer_part.append(1, current_char);
            }
        }
        else {
            break;
        }

    } while (context.Forward());

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


ParseStatus DecimalNumberParser::ParseUnit(ParseContext& context, NumberUnit& unit) {

    auto number_unit = GetNumberUnitByChar(context.GetCurrentChar());
    if (number_unit) {

        unit = *number_unit;

        context.Forward();
        return ParseStatus::Ok;
    }

    unit = NumberUnit::Normal;
    return ParseStatus::Ok;
}

}