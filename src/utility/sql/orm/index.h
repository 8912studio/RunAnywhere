#pragma once

#include "utility/sql/orm/abstract_index.h"
#include "utility/sql/orm/composite_column.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class Index : public CompositeColumn<E, Columns...>, public AbstractIndex {
public:
    Index(
        std::vector<const AbstractIndex*>& registered_indexes, 
        const Columns&... columns) 
        :
        CompositeColumn<E, Columns...>(columns...) {

        registered_indexes.push_back(this);
    }

    AbstractColumnsView GetAbstractColumns() const noexcept override {
        return CompositeColumn<E, Columns...>::GetAbstractColumns();
    }
};

template<typename E, typename... Columns>
auto MakeIndex(std::vector<const AbstractIndex*>& registered_indexes, const Columns&... columns) {
    return Index<E, Columns...>(registered_indexes, columns...);
}

}