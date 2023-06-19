#include "module/calculator/parse/non_decimal_number_parser.h"
#include <zaf/base/container/utility/contain.h>
#include "module/calculator/parse/operand_node.h"

using namespace std::string_literals;

namespace ra::mod::calculator {

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

    auto reader = context.BeginRead();

    if (!ParsePrefix(reader)) {
        reader.Discard();
        return ParseStatus::Mismatched;
    }

    std::wstring number;
    bool is_succeeded = ParseNumber(reader, number);
    if (!is_succeeded) {
        return ParseStatus::Error;
    }

    auto operand_node = std::make_shared<OperandNode>();
    operand_node->base = base_;
    operand_node->text = number;

    parse_result.AddOperand(operand_node);
    return ParseStatus::Ok;
}


bool NonDecimalNumberParser::ParsePrefix(ParseReader& reader) {

     for (const auto& each_prefix : prefixes_) {

        if (IsPrefixMatched(reader, each_prefix)) {
            return true;
        }
    }

    return false;
}


bool NonDecimalNumberParser::IsPrefixMatched(
    ParseReader& reader,
    std::wstring_view prefix) {

    for (std::size_t index = 0; index < prefix.length(); ++index) {

        auto ch = reader.GetChar();
        if (std::tolower(ch) != prefix[index]) {
            return false;
        }

        reader.Forward();
    }

    return true;
}


bool NonDecimalNumberParser::ParseNumber(ParseReader& reader, std::wstring& number) {

    while (true) {

        auto ch = reader.GetChar();
        if (!zaf::Contain(chars_, std::tolower(ch))) {
            break;
        }

        number.append(1, ch);
        reader.Forward();
    }

    return !number.empty();
}

}