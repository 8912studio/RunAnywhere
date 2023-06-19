#pragma once

#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/parse/operand_node.h"

namespace ra::mod::calculator {

class OperandEvaluator : public Evaluator {
public:
    OperandEvaluator(const std::shared_ptr<OperandNode>& operand_node) : 
        operand_node_(operand_node) {

    }

    EvaluateStatus Evaluate(EvaluateResult& result) override;

private:
    std::shared_ptr<OperandNode> operand_node_;
};

}