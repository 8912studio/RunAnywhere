#pragma once

#include <string>
#include <vector>
#include "module/calculator/parse/terminal_parser.h"

namespace ra::module::calculator {

class NonDecimalNumberParser : public TerminalParser {
public:
    static NonDecimalNumberParser* Binary();
    static NonDecimalNumberParser* Hex();

public:
    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override;

private:
    NonDecimalNumberParser(
        const std::vector<std::wstring>& prefixes, 
        const std::wstring& chars,
        int base);

    bool ParsePrefix(ParseReader& reader);
    bool IsPrefixMatched(ParseReader& reader, std::wstring_view prefix);
    bool ParseNumber(ParseReader& reader, std::wstring& number);

private:
    std::vector<std::wstring> prefixes_;
    std::wstring chars_;
    int base_{};
};

}