#pragma once

#include "module/calculator/parse/operand_node.h"
#include "module/calculator/parse/operator_node.h"
#include "module/calculator/parse/modifier.h"

namespace ra::mod::calculator {

class ParseResult {
public:
    ParseResult() = default;
    
    ParseResult(const ParseResult&) = delete;
    ParseResult& operator=(const ParseResult&) = delete;

    ParseResult(ParseResult&&) = default;
    ParseResult& operator=(ParseResult&&) = default;

    void AddOperator(const std::shared_ptr<OperatorNode>& operator_node);
    void AddOperand(const std::shared_ptr<OperandNode>& operand_node);

    std::shared_ptr<SyntaxNode> GetExpressionRootNode();

    Modifier& GetMutableModifier() {
        return modifier_;
    }

    const Modifier& GetModifier() const {
        return modifier_;
    }

private:
    void MergeTopOperator();

private:
    std::vector<std::shared_ptr<OperatorNode>> operator_stack_;
    std::vector<std::shared_ptr<SyntaxNode>> operand_stack_;
    Modifier modifier_;
};

}