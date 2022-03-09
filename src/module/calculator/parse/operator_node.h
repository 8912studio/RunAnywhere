#pragma once

#include "module/calculator/parse/syntax_node.h"

namespace ra::module::calculator {

class OperatorNode : public SyntaxNode {
public:
    enum class Type {

        None,

        LeftParenthesis,
        RightParenthesis,

        Positive,
        Negative,
        Not,

        Plus,
        Minus,
        Multiply,
        Power,
        Divide,
        And,
        Or,
        Xor,
        LeftShift,
        RightShift,
    };

public:
    int GetPriority() const {

        switch (type) {
        case Type::Positive:
        case Type::Negative:
        case Type::Not:
            return 103;

        case Type::Power:
            return 102;

        case Type::Multiply:
        case Type::Divide:
            return 101;

        case Type::Plus:
        case Type::Minus:
            return 100;

        case Type::LeftShift:
        case Type::RightShift:
            return 99;

        case Type::And:
        case Type::Or:
        case Type::Xor:
            return 98;

        default:
            return 0;
        }
    }

public:
    Type type{ Type::None };
};

}