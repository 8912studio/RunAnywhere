#pragma once

#include <string>
#include "module/calculator/parse/number_unit.h"
#include "module/calculator/parse/syntax_node.h"

namespace calculator {

class OperandNode : public SyntaxNode {
public:
    std::size_t base{};
    std::wstring text;
    NumberUnit unit{ NumberUnit::Normal };
};

}