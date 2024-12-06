#pragma once

#include "utility/sql/orm/column.h"
#include "utility/sql/orm/column_value_traits.h"
#include "utility/sql/orm/composite_column.h"
#include "utility/sql/orm/expression_operator.h"

namespace ra::utility::sql {

template<typename T, typename = void>
class Operand;


template<typename T>
class Operand<T, std::enable_if_t<IsColumnV<T>>> {
public:
    explicit Operand(const T& column) : column_(&column) {

    }

    std::string BuildSQL() const {
        return std::string{ column_->GetName() };
    };

    int BindParameters(Statement& statement, int begin_index) const {
        return begin_index;
    }

private:
    const T* column_{};
};


template<typename T>
class Operand<T, std::enable_if_t<IsCompositeColumnV<T>>> {
public:
    explicit Operand(const T& composite_column) : composite_column_(composite_column) {

    }

private:
    T composite_column_;
};


template<typename T>
class Operand<T, std::enable_if_t<HasDataTypeMappingV<T>>> {
public:
    explicit Operand(T value) : value_(std::move(value)) {

    }

    std::string BuildSQL() const {
        return "?";
    }

    int BindParameters(Statement& statement, int begin_index) const {
        ColumnValueTraits<T>::BindValueToStatement(statement, begin_index, value_);
        return begin_index + 1;
    }

private:
    T value_{};
};


template<typename LHS, typename RHS>
class Expression {
private:
    using ThisType = Expression<LHS, RHS>;

public:
    Expression(LHS lhs, RHS rhs, ExpressionOperator expression_operator) : 
        lhs_(std::move(lhs)),
        rhs_(std::move(rhs)),
        expression_operator_(expression_operator) {

    }

    template<typename Other>
    auto operator&&(Other other) const {
        return Expression<ThisType, Other>{ *this, std::move(other), ExpressionOperator::And };
    }

    template<typename Other>
    auto operator||(Other other) const {
        return Expression<ThisType, Other>{ *this, std::move(other), ExpressionOperator::Or };
    }

    std::string BuildSQL() const {
        return std::format(
            "({} {} {})", 
            lhs_.BuildSQL(), 
            ConvertExpressionOperatorToString(expression_operator_),
            rhs_.BuildSQL());
    }

    int BindParameters(Statement& statement, int begin_index) const {
        int index = lhs_.BindParameters(statement, begin_index);
        return rhs_.BindParameters(statement, index);
    }

private:
    LHS lhs_{};
    RHS rhs_{};
    ExpressionOperator expression_operator_{};
};

}