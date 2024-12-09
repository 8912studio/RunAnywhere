#pragma once

#include <format>
#include "utility/sql/orm/base_query.h"
#include "utility/sql/orm/limit_query.h"
#include "utility/sql/orm/order_by_query.h"
#include "utility/sql/orm/select_columns_query_core.h"
#include "utility/sql/orm/where_query.h"

namespace ra::utility::sql {

template<typename Core>
class SelectQuery : public BaseQuery<SelectQuery<Core>> {
private:
    friend class BaseQuery<SelectQuery<Core>>;

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    SelectQuery(Core core) : core_(std::move(core)) {

    }

    template<typename Expr>
    auto Where(Expr expression) const {
        return WhereQuery<Core, Expr>(core_, std::move(expression));
    }

    template<typename... OrderColumns>
    auto OrderBy(const OrderColumns&... columns) const {
        return OrderByQuery<Core, OrderColumns...>(core_, columns...);
    }

    auto Limit(std::size_t limit) const {
        return LimitQuery<Core>(core_, limit);
    }
};

}