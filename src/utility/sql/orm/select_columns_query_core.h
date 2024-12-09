#pragma once

#include "utility/sql/orm/base_select_query_core.h"
#include "utility/sql/orm/composite_column.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class SelectColumnsQueryCore : public BaseSelectQueryCore<SelectColumnsQueryCore<E, Columns...>> {
private:
    friend class BaseSelectQueryCore<SelectColumnsQueryCore<E, Columns...>>;

    using CompositeColumnType = CompositeColumn<E, Columns...>;
    CompositeColumnType composite_column_;

    AbstractColumnsView GetAbstractColumns() const {
        return composite_column_.GetAbstractColumns();
    }

public:
    using EntityType = E;
    using ResultElementType = typename CompositeColumnType::ValueType;

public:
    SelectColumnsQueryCore(Database& db, const Columns&... columns) : 
        BaseSelectQueryCore<SelectColumnsQueryCore<E, Columns...>>(db),
        composite_column_(columns...) {

    }

    ResultElementType GetElement(const Statement& statement) const {
        return CompositeColumnType::GetValueFromStatement(statement, 0);
    }
};

}