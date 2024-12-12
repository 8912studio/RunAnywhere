#pragma once

#include <format>
#include "utility/sql/orm/expression/operand.h"
#include "utility/sql/orm/expression/operator.h"

namespace ra::utility::sql {

template<Operator OP, typename LHS, typename RHS>
class Expression {
private:
    using ThisType = Expression<OP, LHS, RHS>;

public:
    Expression(LHS lhs, RHS rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {

    }

    template<typename Other>
    auto operator&&(Other other) const {
        using ResultType = Expression<Operator::And, ThisType, Other>;
        return ResultType{ *this, std::move(other) };
    }

    template<typename Other>
    auto operator||(Other other) const {
        using ResultType = Expression<Operator::Or, ThisType, Other>;
        return ResultType{ *this, std::move(other) };
    }

    std::string BuildSQL() const {
        return std::format(
            "({}{}{})",
            lhs_.BuildSQL(), 
            ConvertOperatorTagToString(OP),
            rhs_.BuildSQL());
    }

    int BindParameters(Statement& statement, int begin_index) const {
        int index = lhs_.BindParameters(statement, begin_index);
        return rhs_.BindParameters(statement, index);
    }

private:
    LHS lhs_{};
    RHS rhs_{};
};


template<Operator Operator, typename Operand1, typename Operand2>
auto MakeExpression(Operand1&& op1, Operand2&& op2) {
    using LHS = Operand<std::decay_t<Operand1>>;
    using RHS = Operand<std::decay_t<Operand2>>;
    return Expression<Operator, LHS, RHS>{
        LHS{ std::forward<Operand1>(op1) },
        RHS{ std::forward<Operand2>(op2) },
    };
}

}