#pragma once

#include "utility/sql/orm/abstract_primary_key.h"
#include "utility/sql/orm/composite_column.h"

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
};


template<typename E, typename... Columns>
PrimaryKey<E, std::decay_t<Columns>...> DeducePrimaryKeyType(const Columns&... columns) {}

}