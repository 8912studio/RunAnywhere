#pragma once

#include "module/calculator/parse/non_terminal_parser.h"

namespace ra::module::calculator {

class ModifierParser : public NonTerminalParser {
public:
    static ModifierParser* Instance();

protected:
    void InitializeParsers() override;

private:
    ModifierParser() = default;
};

}