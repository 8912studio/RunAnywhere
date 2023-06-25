#include "module/calculator/parse/parse_result.h"

namespace ra::mod::calculator {
namespace {

bool IsUnaryOperator(OperatorNode::Type type) {

    switch (type) {
    case OperatorNode::Type::Positive:
    case OperatorNode::Type::Negative:
    case OperatorNode::Type::Not:
        return true;
    default:
        return false;
    }
}


bool IsBinaryOperator(OperatorNode::Type type) {

    switch (type) {
    case OperatorNode::Type::Plus:
    case OperatorNode::Type::Minus:
    case OperatorNode::Type::Multiply:
    case OperatorNode::Type::Divide:
    case OperatorNode::Type::Power:
    case OperatorNode::Type::And:
    case OperatorNode::Type::Or:
    case OperatorNode::Type::Xor:
    case OperatorNode::Type::LeftShift:
    case OperatorNode::Type::RightShift:
        return true;
    default:
        return false;
    }
}

}

void ParseResult::AddOperator(const std::shared_ptr<OperatorNode>& operator_node) {

    if (operator_stack_.empty() || operator_node->type == OperatorNode::Type::LeftParenthesis) {
        operator_stack_.push_back(operator_node);
        return;
    }

    if (operator_node->type == OperatorNode::Type::RightParenthesis) {

        while (true) {

            const auto& top_operator = operator_stack_.back();
            if (top_operator->type != OperatorNode::Type::LeftParenthesis) {
                MergeTopOperator();
            }
            else {
                operator_stack_.pop_back();
                break;
            }
        }
        return;
    }

    auto top_operator = operator_stack_.back();

    if (top_operator->type == OperatorNode::Type::LeftParenthesis ||
        top_operator->GetPriority() < operator_node->GetPriority()) {
        operator_stack_.push_back(operator_node);
        return;
    }

    //Top operator and new operator are both unary operators, it means that the new one is the 
    //operand of the top one, such as an expression "--1", so we push the new one into the stack.
    if (IsUnaryOperator(top_operator->type) && IsUnaryOperator(operator_node->type)) {
        operator_stack_.push_back(operator_node);
        return;
    }

    while (!operator_stack_.empty()) {

        top_operator = operator_stack_.back();

        if (top_operator->type == OperatorNode::Type::LeftParenthesis) {
            break;
        }
        else if (top_operator->GetPriority() >= operator_node->GetPriority()) {
            MergeTopOperator();
        }
        else {
            break;
        }
    }

    operator_stack_.push_back(operator_node);
}


void ParseResult::AddOperand(const std::shared_ptr<OperandNode>& operand_node) {
    operand_stack_.push_back(operand_node);
}


std::shared_ptr<SyntaxNode> ParseResult::GetExpressionRootNode() {

    while (!operator_stack_.empty()) {
        MergeTopOperator();
    }

    if (!operand_stack_.empty()) {
        return operand_stack_.back(); 
    }

    return nullptr;
}


void ParseResult::MergeTopOperator() {

    auto top_operator = operator_stack_.back();
    operator_stack_.pop_back();

    std::size_t operand_count = 0;
    if (IsUnaryOperator(top_operator->type)) {
        operand_count = 1;
    }
    else if (IsBinaryOperator(top_operator->type)) {
        operand_count = 2;
    }

    std::vector<std::shared_ptr<SyntaxNode>> operands;

    for (std::size_t current_count = 0; current_count < operand_count; ++current_count) {
        operands.push_back(operand_stack_.back());
        operand_stack_.pop_back();
    }

    std::reverse(operands.begin(), operands.end());

    top_operator->children = operands;
    operand_stack_.push_back(top_operator);
}

}