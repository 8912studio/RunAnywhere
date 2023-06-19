#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace ra::mod::calculator {

class BinaryExpressionParser : public NonTerminalParser {
public:
    static BinaryExpressionParser* Instance();

protected:
    void InitializeParsers() override;

private:
    BinaryExpressionParser() = default;
};

}