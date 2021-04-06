#include "module/calculator/evaluate/operator_evaluator.h"
#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/operator_node.h"

namespace calculator {

class UnaryOperatorEvaluator : public OperatorEvaluator {
public:
    using OperatorEvaluator::OperatorEvaluator;

    EvaluateStatus EvaluateChildren() override {

        auto child_evalutor = Evaluator::Create(GetOperatorNode()->children[0]);
        return child_evalutor->Evaluate(child_value_);
    }

    const EvaluateResult& GetChildValue() const {
        return child_value_;
    }

private:
    EvaluateResult child_value_;
};


class BinaryOperatorEvaluator : public OperatorEvaluator {
public:
    using OperatorEvaluator::OperatorEvaluator;

    EvaluateStatus EvaluateChildren() override {

        auto child_evalutor = Evaluator::Create(GetOperatorNode()->children[0]);
        auto status = child_evalutor->Evaluate(first_child_value_);
        if (status != EvaluateStatus::Ok) {
            return status;
        }

        child_evalutor = Evaluator::Create(GetOperatorNode()->children[1]);
        return child_evalutor->Evaluate(second_child_value_);
    }

    const EvaluateResult& GetFirstChildValue() const {
        return first_child_value_;
    }

    const EvaluateResult& GetSecondChildValue() const {
        return second_child_value_;
    }

private:
    EvaluateResult first_child_value_;
    EvaluateResult second_child_value_;
};


class PositiveOperatorEvaluator : public UnaryOperatorEvaluator {
public:
    using UnaryOperatorEvaluator::UnaryOperatorEvaluator;

    EvaluateStatus EvaluateValue(EvaluateResult& result) override {
        result = GetChildValue();
        return EvaluateStatus::Ok;
    }
};


class NegativeOperatorEvaluator : public UnaryOperatorEvaluator {
public:
    using UnaryOperatorEvaluator::UnaryOperatorEvaluator;

    EvaluateStatus EvaluateValue(EvaluateResult& result) override {
        result = GetChildValue();
        result.decimal_value *= -1;
        return EvaluateStatus::Ok;
    }
};


class NotOperatorEvaluator : public UnaryOperatorEvaluator {
public:
    using UnaryOperatorEvaluator::UnaryOperatorEvaluator;

    EvaluateStatus EvaluateValue(EvaluateResult& result) override {

        result = GetChildValue();

        auto integer_value = result.decimal_value.convert_to<std::int64_t>();
        integer_value = ~integer_value;
        result.decimal_value = integer_value;

        return EvaluateStatus::Ok;
    }
};


class PlusOperatorEvaluator : public BinaryOperatorEvaluator {
public:
    using BinaryOperatorEvaluator::BinaryOperatorEvaluator;

    EvaluateStatus EvaluateValue(EvaluateResult& result) override {
        result = GetFirstChildValue();
        result.decimal_value += GetSecondChildValue().decimal_value;
        return EvaluateStatus::Ok;
    }
};


class MinusOperatorEvaluator : public BinaryOperatorEvaluator {
public:
    using BinaryOperatorEvaluator::BinaryOperatorEvaluator;

    EvaluateStatus EvaluateValue(EvaluateResult& result) override {
        result = GetFirstChildValue();
        result.decimal_value -= GetSecondChildValue().decimal_value;
        return EvaluateStatus::Ok;
    }
};


class MultiplyOperatorEvaluator : public BinaryOperatorEvaluator {
public:
    using BinaryOperatorEvaluator::BinaryOperatorEvaluator;

    EvaluateStatus EvaluateValue(EvaluateResult& result) override {
        result = GetFirstChildValue();
        result.decimal_value *= GetSecondChildValue().decimal_value;
        return EvaluateStatus::Ok;
    }
};


class DivideOperatorEvaluator : public BinaryOperatorEvaluator {
public:
    using BinaryOperatorEvaluator::BinaryOperatorEvaluator;

    EvaluateStatus EvaluateValue(EvaluateResult& result) override {
        result = GetFirstChildValue();
        result.decimal_value /= GetSecondChildValue().decimal_value;
        return EvaluateStatus::Ok;
    }
};


std::shared_ptr<OperatorEvaluator> OperatorEvaluator::Create(
    const std::shared_ptr<OperatorNode>& operator_node) {

    switch (operator_node->type) {
    case OperatorNode::Type::Positive:
        return std::make_shared<PositiveOperatorEvaluator>(operator_node);

    case OperatorNode::Type::Negative:
        return  std::make_shared<NegativeOperatorEvaluator>(operator_node);

    case OperatorNode::Type::Not:
        return std::make_shared<NotOperatorEvaluator>(operator_node);

    case OperatorNode::Type::Plus:
        return std::make_shared<PlusOperatorEvaluator>(operator_node);

    case OperatorNode::Type::Minus:
        return std::make_shared<MinusOperatorEvaluator>(operator_node);

    case OperatorNode::Type::Multiply:
        return std::make_shared<MultiplyOperatorEvaluator>(operator_node);

    case OperatorNode::Type::Divide:
        return std::make_shared<DivideOperatorEvaluator>(operator_node);

    default:
        return nullptr;
    }
}


EvaluateStatus OperatorEvaluator::Evaluate(EvaluateResult& result) {

    auto status = EvaluateChildren();
    if (status != EvaluateStatus::Ok) {
        return status;
    }

    return EvaluateValue(result);
}

}