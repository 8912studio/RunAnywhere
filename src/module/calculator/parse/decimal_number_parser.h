#pragma once

#include "module/calculator/parse/terminal_parser.h"
#include "module/calculator/parse/number_unit.h"

namespace ra::mod::calculator {

class DecimalNumberParser : public TerminalParser {
public:
    static DecimalNumberParser* Instance();

    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override;

private:
    DecimalNumberParser() = default;

    ParseStatus ParseNumberParts(
        ParseReader& reader,
        std::wstring& integer_part,
        std::wstring& float_part);

    void ParseUnit(ParseReader& reader, NumberUnit& unit);

    std::shared_ptr<OperandNode> CreateOperandNode(
        const std::wstring& integer_part,
        const std::wstring& float_part, 
        NumberUnit unit);
};

}