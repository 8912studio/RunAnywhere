#pragma once

#include "utility/sql/orm/abstract_primary_key.h"
#include "utility/sql/orm/composite_column.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class PrimaryKey : public CompositeColumn<E, Columns...>, public AbstractPrimaryKey {
public:
    static_assert(
        !(Columns::ValueTraits::IsNullable && ...), 
        "Primary key columns cannot be nullable.");

public:
    using CompositeColumn<E, Columns...>::CompositeColumn;

    AbstractColumnsView GetAbstractColumns() const noexcept override {
        return CompositeColumn<E, Columns...>::GetAbstractColumns();
    }
};


template<typename E, typename... Columns>
auto MakePrimaryKey(Columns&... columns) {
    return PrimaryKey<E, Columns...>(columns...);
}

}