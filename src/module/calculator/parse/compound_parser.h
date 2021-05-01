#pragma once

#include <memory>
#include <vector>
#include "module/calculator/parse/parser.h"

namespace ra::module::calculator {

class CompoundParser : public Parser {
public:
    void AddParser(Parser* parser) {
        parsers_.push_back(parser);
    }

    ParseStatus Parse(ParseContext& context, ParseResult& result) override;

private:
    std::vector<Parser*> parsers_;
};

}