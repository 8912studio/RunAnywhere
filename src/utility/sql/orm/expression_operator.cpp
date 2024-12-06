#include "utility/sql/orm/expression_operator.h"

namespace ra::utility::sql {

std::string_view ConvertExpressionOperatorToString(ExpressionOperator expression_operator) {
    switch (expression_operator) {
    case ExpressionOperator::Equal:
        return "=";
    case ExpressionOperator::NotEqual:
        return "<>";
    case ExpressionOperator::Less:
        return "<";
    case ExpressionOperator::LessEqual:
        return "<=";
    case ExpressionOperator::Greater:
        return ">";
    case ExpressionOperator::GreaterEqual:
        return ">=";
    case ExpressionOperator::And:
        return "and";
    case ExpressionOperator::Or:
        return "or";
    default:
        return "";
    }
}

}