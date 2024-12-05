#pragma once

#include "utility/sql/orm/base_query.h"
#include "utility/sql/orm/composite_column.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/decorative_query_core.h"
#include "utility/sql/orm/limit_query.h"

namespace ra::utility::sql {

template<typename InnerCore, typename... Columns>
class OrderByQuery : public BaseQuery<OrderByQuery<InnerCore, Columns...>> {
private:
    friend class BaseQuery<OrderByQuery<InnerCore, Columns...>>;

    using CompositeColumnType = CompositeColumn<typename InnerCore::EntityType, Columns...>;

    class Core : public DecorativeQueryCore<InnerCore> {
    public:
        Core(InnerCore inner_core, const Columns&... columns) : 
            DecorativeQueryCore<InnerCore>(std::move(inner_core)),
            composite_column_(columns...) {

        }

        std::string BuildSQL() const {

            auto sql = this->GetInnerCore().BuildSQL();

            sql += std::format(
                " order by {}",
                JoinColumnNames(composite_column_.GetAbstractColumns()));

            return sql;
        }

    private:
        CompositeColumnType composite_column_;
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    OrderByQuery(InnerCore inner_core, const Columns&... columns) : 
        core_(std::move(inner_core), columns...) {

    }

    LimitQuery<Core> Limit(std::size_t limit) const {
        return LimitQuery<Core>(core_, limit);
    }
};

}