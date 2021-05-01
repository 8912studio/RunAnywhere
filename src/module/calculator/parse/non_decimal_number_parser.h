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

    ParseStatus ParsePrefix(ParseContext& context, std::size_t& prefix_length);
    ParseStatus ParseNumber(ParseContext& context, std::wstring& number);

private:
    std::vector<std::wstring> prefixes_;
    std::wstring chars_;
    int base_{};
};

}