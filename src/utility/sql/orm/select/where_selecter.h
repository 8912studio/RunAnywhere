#pragma once

#include "utility/sql/orm/select/selecter.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/select/decorative_selecter_core.h"
#include "utility/sql/orm/expression.h"
#include "utility/sql/orm/select/limit_selecter.h"
#include "utility/sql/orm/select/order_by_selecter.h"

namespace ra::utility::sql {

template<typename InnerCore, typename Expr>
class WhereSelecter : public Selecter<WhereSelecter<InnerCore, Expr>> {
private:
    friend class Selecter<WhereSelecter<InnerCore, Expr>>;

    class Core : public DecorativeSelecterCore<InnerCore> {
    public:
        Core(InnerCore inner_core, Expr expression) : 
            DecorativeSelecterCore<InnerCore>(std::move(inner_core)),
            expression_(std::move(expression)) {

        }

        std::string BuildSQL() const {
            auto sql = this->GetInnerCore().BuildSQL();
            sql += std::format(" where {}", expression_.BuildSQL());
            return sql;
        }

        int BindParameters(Statement& statement, int begin_index) const {
            int index = this->GetInnerCore().BindParameters(statement, begin_index);
            return expression_.BindParameters(statement, index);
        }

    private:
        Expr expression_;
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    WhereSelecter(InnerCore inner_core, Expr expression) : 
        core_(std::move(inner_core), std::move(expression)) {

    }

    template<typename... OrderColumns>
    auto OrderBy(const OrderColumns&... columns) const {
        return OrderBySelecter<Core, OrderColumns...>(core_, columns...);
    }

    auto Limit(std::size_t limit) const {
        return LimitSelecter<Core>(core_, limit);
    }
};

}