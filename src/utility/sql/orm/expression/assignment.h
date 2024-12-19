#pragma once

#include "utility/sql/orm/expression/operand.h"
#include "utility/sql/statement.h"

namespace ra::utility::sql {

template<typename LHS, typename RHS>
class Assignment {
public:
    Assignment(LHS lhs, RHS rhs) : lhs_(std::move(lhs)), rhs_(std::move(rhs)) {

    }

    std::string BuildSQL() const {
        return std::format("{}={}", lhs_.BuildSQL(), rhs_.BuildSQL());
    }

    int BindParameters(Statement& statement, int begin_index) const {
        int index = lhs_.BindParameters(statement, begin_index);
        return rhs_.BindParameters(statement, index);
    }

private:
    LHS lhs_{};
    RHS rhs_{};
};


template<typename Operand1, typename Operand2>
auto MakeAssignment(Operand1&& op1, Operand2&& op2) {
    using LHS = Operand<std::decay_t<Operand1>>;
    using RHS = Operand<std::decay_t<Operand2>>;
    return Assignment<LHS, RHS>{
        LHS{ std::forward<Operand1>(op1) },
        RHS{ std::forward<Operand2>(op2) },
    };
}

}