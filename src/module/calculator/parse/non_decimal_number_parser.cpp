#include "module/calculator/parse/non_decimal_number_parser.h"
#include <zaf/base/container/utility/contain.h>
#include "module/calculator/parse/operand_node.h"

using namespace std::string_literals;

namespace ra::module::calculator {

NonDecimalNumberParser* NonDecimalNumberParser::Binary() {
    static NonDecimalNumberParser parser{ { L"0b", L"b" }, L"01", 2 };
    return &parser;
}


NonDecimalNumberParser* NonDecimalNumberParser::Hex() {
    static NonDecimalNumberParser parser{ { L"0x", L"x" }, L"0123456789abcedf", 16 };
    return &parser;
}


NonDecimalNumberParser::NonDecimalNumberParser(
    const std::vector<std::wstring>& prefixes,
    const std::wstring& chars,
    int base)
    :
    prefixes_(prefixes),
    chars_(chars),
    base_(base) {

}


ParseStatus NonDecimalNumberParser::Parse(ParseContext& context, ParseResult& parse_result) {

    std::size_t prefix_length{};
    auto parse_status = ParsePrefix(context, prefix_length);
    if (parse_status != ParseStatus::Ok) {
        return parse_status;
    }

    std::wstring number;
    parse_status = ParseNumber(context, number);
    if (parse_status != ParseStatus::Ok) {

        if (parse_status == ParseStatus::Mismatched) {
            context.Backward(prefix_length);
        }
        return parse_status;
    }

    auto operand_node = std::make_shared<OperandNode>();
    operand_node->base = base_;
    operand_node->text = number;

    parse_result.AddOperand(operand_node);
    return ParseStatus::Ok;
}


ParseStatus NonDecimalNumberParser::ParsePrefix(ParseContext& context, std::size_t& prefix_length) {

    for (const auto& each_prefix : prefixes_) {

        std::size_t index = 0;
        for (; index < each_prefix.length(); ++index) {

            if (std::tolower(context.GetCurrentChar()) != each_prefix[index]) {
                break;
            }

            if (!context.Forward()) {

                if (index == 0) {
                    return ParseStatus::Mismatched;
                }
                else {
                    return ParseStatus::Error;
                }
            }
        }

        if (index == each_prefix.length()) {
            prefix_length = index;
            return ParseStatus::Ok;
        }

        if (index > 0) {
            context.Backward(index);
        }
    }

    return ParseStatus::Mismatched;
}


ParseStatus NonDecimalNumberParser::ParseNumber(ParseContext& context, std::wstring& number) {

    do {

        wchar_t current_char = context.GetCurrentChar();
        if (!zaf::Contain(chars_, std::tolower(current_char))) {
            break;
        }

        number.append(1, current_char);
    } 
    while (context.Forward());

    if (!number.empty()) {
        return ParseStatus::Ok;
    }

    return ParseStatus::Error;
}

}