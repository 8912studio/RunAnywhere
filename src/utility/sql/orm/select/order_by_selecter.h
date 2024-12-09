#pragma once

#include "utility/sql/orm/select/selecter.h"
#include "utility/sql/orm/composite_column.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/select/decorative_selecter_core.h"
#include "utility/sql/orm/select/limit_selecter.h"

namespace ra::utility::sql {

template<typename InnerCore, typename... Columns>
class OrderBySelecter : public Selecter<OrderBySelecter<InnerCore, Columns...>> {
private:
    friend class Selecter<OrderBySelecter<InnerCore, Columns...>>;

    class Core : public DecorativeSelecterCore<InnerCore> {
    public:
        Core(InnerCore inner_core, const Columns&... columns) : 
            DecorativeSelecterCore<InnerCore>(std::move(inner_core)),
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
        CompositeColumn<typename InnerCore::EntityType, Columns...> composite_column_;
    };

    const Core& GetCore() const {
        return core_;
    }

    Core core_;

public:
    OrderBySelecter(InnerCore inner_core, const Columns&... columns) :
        core_(std::move(inner_core), columns...) {

    }

    LimitSelecter<Core> Limit(std::size_t limit) const {
        return LimitSelecter<Core>(core_, limit);
    }
};

}