#pragma once

#include "utility/sql/orm/abstract_primary_key.h"
#include "utility/sql/orm/key.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class PrimaryKey : public Key<E, Columns...>, public AbstractPrimaryKey {
public:
    static_assert(
        !(Columns::ValueTraits::IsNullable && ...), 
        "Primary key columns cannot be nullable.");

public:
    using Key<E, Columns...>::Key;

    AbstractColumnsView GetAbstractColumns() const noexcept override {
        return Key<E, Columns...>::GetAbstractColumns();
    }
};


template<typename E, typename... Columns>
auto MakePrimaryKey(Columns&... columns) {
    return PrimaryKey<E, Columns...>(columns...);
}

}