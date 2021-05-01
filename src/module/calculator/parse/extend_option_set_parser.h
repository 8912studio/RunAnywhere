#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace ra::module::calculator {

class ExtendOptionSetParser : public NonTerminalParser {
public:
    static ExtendOptionSetParser* Instance();

    ParseStatus Parse(ParseContext& context, ParseResult& result) override;

protected:
    void InitializeParsers() override {

    }

private:
    ExtendOptionSetParser() = default;
};

}