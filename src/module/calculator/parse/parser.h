#pragma once

#include <memory>
#include "module/calculator/parse/parse_context.h"
#include "module/calculator/parse/parse_result.h"
#include "module/calculator/parse/parse_status.h"

namespace ra::mod::calculator {

class Parser {
public:
    Parser() = default;
    virtual ~Parser() = default;

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;

    virtual ParseStatus Parse(ParseContext& context, ParseResult& result) = 0;
};

}