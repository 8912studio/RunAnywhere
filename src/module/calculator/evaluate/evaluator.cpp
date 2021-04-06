#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/evaluate/operand_evaluator.h"
#include "module/calculator/evaluate/operator_evaluator.h"
#include "module/calculator/parse/operand_node.h"
#include "module/calculator/parse/operator_node.h"

namespace calculator {

std::shared_ptr<Evaluator> Evaluator::Create(const std::shared_ptr<SyntaxNode>& syntax_node) {

    auto operand_node = std::dynamic_pointer_cast<OperandNode>(syntax_node);
    if (operand_node) {
        return std::make_shared<OperandEvaluator>(operand_node);
    }

    auto operator_node = std::dynamic_pointer_cast<OperatorNode>(syntax_node);
    if (operator_node) {
        return OperatorEvaluator::Create(operator_node);
    }

    return nullptr;
}

}