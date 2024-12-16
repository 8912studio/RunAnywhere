#pragma once

#include "utility/sql/orm/abstract_primary_key.h"
#include "utility/sql/orm/composite_column.h"
#include "utility/sql/orm/expression/expression_support.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class PrimaryKey : public CompositeColumn<E, Columns...>, public AbstractPrimaryKey {
    static_assert(
        !(Columns::ValueTraits::IsNullable && ...), 
        "Primary key columns cannot be nullable.");

private:
    using CompositeColumnType = CompositeColumn<E, Columns...>;

public:
    using CompositeColumnType::CompositeColumn;

    AbstractColumnsView GetAbstractColumns() const noexcept override {
        return CompositeColumn<E, Columns...>::GetAbstractColumns();
    }

    SQL_EXPRESSION_OPERATORS(CompositeColumnType, CompositeColumnType::ValueType)
};


template<typename E, typename... Columns>
auto MakePrimaryKey(Columns&... columns) {
    return PrimaryKey<E, Columns...>(columns...);
}

}