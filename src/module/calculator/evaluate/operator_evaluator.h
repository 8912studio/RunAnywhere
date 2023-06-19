#pragma once

#include "module/calculator/evaluate/evaluator.h"
#include "module/calculator/parse/operator_node.h"

namespace ra::mod::calculator {

class OperatorEvaluator : public Evaluator {
public:
    static std::shared_ptr<OperatorEvaluator> Create(
        const std::shared_ptr<OperatorNode>& operator_node);

public:
    OperatorEvaluator(const std::shared_ptr<OperatorNode>& operator_node) :
        operator_node_(operator_node) {

    }

    EvaluateStatus Evaluate(EvaluateResult& result) override;

protected:
    const std::shared_ptr<OperatorNode>& GetOperatorNode() const {
        return operator_node_;
    }

    virtual EvaluateStatus EvaluateChildren() = 0;
    virtual EvaluateStatus EvaluateValue(EvaluateResult& result) = 0;

private:
    std::shared_ptr<OperatorNode> operator_node_;
};

}