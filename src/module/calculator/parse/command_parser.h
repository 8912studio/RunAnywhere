#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace calculator {

class CommandParser : public NonTerminalParser {
public:
    static CommandParser* Instance();

    ParseStatus Parse(ParseContext& context, ParseResult& result) override;

protected:
    void InitializeParsers() override;

private:
    CommandParser() = default;
};

}