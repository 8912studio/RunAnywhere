#pragma once

#include "utility/sql/orm/expression/operand.h"
#include "utility/sql/orm/expression/operator.h"

namespace ra::utility::sql {

template<typename LHS, typename RHS>
class Expression {
private:
    using ThisType = Expression<LHS, RHS>;

public:
    Expression(LHS lhs, RHS rhs, Operator op) : 
        lhs_(std::move(lhs)),
        rhs_(std::move(rhs)),
        operator_(op) {

    }

    template<typename Other>
    auto operator&&(Other other) const {
        return Expression<ThisType, Other>{ *this, std::move(other), Operator::And };
    }

    template<typename Other>
    auto operator||(Other other) const {
        return Expression<ThisType, Other>{ *this, std::move(other), Operator::Or };
    }

    std::string BuildSQL() const {
        return std::format(
            "({} {} {})", 
            lhs_.BuildSQL(), 
            ConvertOperatorToString(operator_),
            rhs_.BuildSQL());
    }

    int BindParameters(Statement& statement, int begin_index) const {
        int index = lhs_.BindParameters(statement, begin_index);
        return rhs_.BindParameters(statement, index);
    }

private:
    LHS lhs_{};
    RHS rhs_{};
    Operator operator_{};
};

}