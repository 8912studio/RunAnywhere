#pragma once

#include "utility/sql/orm/expression/assignment.h"
#include "utility/sql/orm/expression/expression.h"

#define SQL_EXPRESSION_OPERATORS(COLUMN_TYPE, VALUE_TYPE) \
auto operator=(const VALUE_TYPE& value) const { \
    return ra::utility::sql::MakeAssignment(*this, value); \
} \
friend auto operator==(const COLUMN_TYPE& column, const VALUE_TYPE& value) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::Equal>( \
        column, value); \
} \
friend auto operator==(const VALUE_TYPE& value, const COLUMN_TYPE& column) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::Equal>( \
        value, column); \
} \
friend auto operator!=(const COLUMN_TYPE& column, const VALUE_TYPE& value) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::NotEqual>( \
        column, value); \
} \
friend auto operator!=(const VALUE_TYPE& value, const COLUMN_TYPE& column) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::NotEqual>( \
        value, column); \
} \
friend auto operator<(const COLUMN_TYPE& column, const VALUE_TYPE& value) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::Less>( \
        column, value); \
} \
friend auto operator<(const VALUE_TYPE& value, const COLUMN_TYPE& column) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::Less>( \
        value, column); \
} \
friend auto operator<=(const COLUMN_TYPE& column, const VALUE_TYPE& value) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::LessEqual>( \
        column, value); \
} \
friend auto operator<=(const VALUE_TYPE& value, const COLUMN_TYPE& column) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::LessEqual>( \
        value, column); \
} \
friend auto operator>(const COLUMN_TYPE& column, const VALUE_TYPE& value) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::Greater>( \
        column, value); \
} \
friend auto operator>(const VALUE_TYPE& value, const COLUMN_TYPE& column) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::Greater>( \
        value, column); \
} \
friend auto operator>=(const COLUMN_TYPE& column, const VALUE_TYPE& value) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::GreaterEqual>( \
        column, value); \
} \
friend auto operator>=(const VALUE_TYPE& value, const COLUMN_TYPE& column) { \
    return ra::utility::sql::MakeExpression<ra::utility::sql::Operator::GreaterEqual>( \
        value, column); \
}