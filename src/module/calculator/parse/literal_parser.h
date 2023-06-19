#pragma once

#include <string>
#include "module/calculator/parse/parser.h"

namespace ra::mod::calculator {

class LiteralParser : public Parser {
public:
    LiteralParser(const std::wstring& literal) : literal_(literal) { }

    ParseStatus Parse(ParseContext& context, ParseResult& parse_result) override;

private:
    std::wstring literal_;
};

}