#pragma once

#include "utility/sql/orm/base_query.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/decorative_query_core.h"
#include "utility/sql/orm/expression.h"
#include "utility/sql/orm/limit_query.h"
#include "utility/sql/orm/order_by_query.h"

namespace ra::utility::sql {

template<typename InnerCore, typename Expr>
class WhereQuery : public BaseQuery<WhereQuery<InnerCore, Expr>> {
private:
    friend class BaseQuery<WhereQuery<InnerCore, Expr>>;

    class Core : public DecorativeQueryCore<InnerCore> {
    public:
        Core(InnerCore inner_core, Expr expression) : 
            DecorativeQueryCore<InnerCore>(std::move(inner_core)),
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
    WhereQuery(InnerCore inner_core, Expr expression) : 
        core_(std::move(inner_core), std::move(expression)) {

    }
};

}