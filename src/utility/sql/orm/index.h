#pragma once

#include "utility/sql/orm/abstract_index.h"
#include "utility/sql/orm/key.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class Index : public Key<E, Columns...>, public AbstractIndex {
public:
    using Key<E, Columns...>::Key;

    AbstractColumnsView GetAbstractColumns() const noexcept override {
        return Key<E, Columns...>::GetAbstractColumns();
    }
};

template<typename E, typename... Columns>
auto MakeIndex(Columns&... columns) {
    return Index<E, Columns...>(columns...);
}

}