#pragma once

#include <format>
#include "utility/sql/orm/select/column_selecter_core.h"
#include "utility/sql/orm/select/limit_selecter.h"
#include "utility/sql/orm/select/order_by_selecter.h"
#include "utility/sql/orm/select/selecter.h"
#include "utility/sql/orm/select/where_selecter.h"

namespace ra::utility::sql {

template<typename Core>
class PrimitiveSelecter : public Selecter<PrimitiveSelecter<Core>> {
public:
    explicit PrimitiveSelecter(Core core) : core_(std::move(core)) {

    }

    template<typename Expr>
    auto Where(Expr expression) const {
        return WhereSelecter<Core, Expr>(core_, std::move(expression));
    }

    template<typename... OrderColumns>
    auto OrderBy(const OrderColumns&... columns) const {
        return OrderBySelecter<Core, OrderColumns...>(core_, columns...);
    }

    auto Limit(std::size_t limit) const {
        return LimitSelecter<Core>(core_, limit);
    }

private:
    friend class Selecter<PrimitiveSelecter<Core>>;

    const Core& GetCore() const {
        return core_;
    }

    Core core_;
};

}