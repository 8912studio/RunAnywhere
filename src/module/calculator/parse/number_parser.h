#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace calculator {

class NumberParser : public NonTerminalParser {
public:
    static NumberParser* Instance();

protected:
    void InitializeParsers() override;

private:
    NumberParser() = default;
};

}