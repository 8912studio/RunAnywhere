#pragma once

#include "module/calculator/parse/terminal_parser.h"

namespace ra::mod::calculator {

class OctalNumberParser : public TerminalParser {
public:
    static OctalNumberParser* Instance();

    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override;

private:
    OctalNumberParser() = default;

    ParseStatus ParseNumber(ParseContext& context, std::wstring& number);
};

}