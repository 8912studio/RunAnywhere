#pragma once

#include <memory>
#include <vector>
#include "module/calculator/parse/compound_parser.h"

namespace calculator {

class NonTerminalParser : public Parser {
public:
    ParseStatus Parse(ParseContext& context, ParseResult& result) override;

protected:
    void AddParsers(const std::vector<Parser*>& parsers);

    virtual void InitializeParsers() = 0;

private:
    bool has_initialize_parsers_{};
    std::vector<std::shared_ptr<CompoundParser>> compound_parsers_;
};

}