#pragma once

#include "module/calculator/parse/terminal_parser.h"
#include "module/calculator/parse/number_unit.h"

namespace ra::module::calculator {

class DecimalNumberParser : public TerminalParser {
public:
    static DecimalNumberParser* Instance();

    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override;

private:
    DecimalNumberParser() = default;

    ParseStatus ParseNumberParts(
        ParseContext& context,
        std::wstring& integer_part,
        std::wstring& float_part);

    ParseStatus ParseUnit(ParseContext& context, NumberUnit& unit);
};

}