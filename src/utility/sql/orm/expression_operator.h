#pragma once

#include <string_view>

namespace ra::utility::sql {

enum class ExpressionOperator {
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,
    And,
    Or,
};

std::string_view ConvertExpressionOperatorToString(ExpressionOperator expression_operator);

}