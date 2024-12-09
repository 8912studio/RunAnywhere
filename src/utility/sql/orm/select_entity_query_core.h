#pragma once

#include <zaf/base/range.h>
#include "utility/sql/orm/base_select_query_core.h"

namespace ra::utility::sql {

template<typename E>
class SelectEntityQueryCore : public BaseSelectQueryCore<SelectEntityQueryCore<E>> {
private:
    friend class BaseSelectQueryCore<SelectEntityQueryCore<E>>;

    AbstractColumnsView GetAbstractColumns() const {
        return E::TableType::GetInstance().GetAbstractColumns();
    }

public:
    using EntityType = E;
    using ResultElementType = E;

public:
    explicit SelectEntityQueryCore(Database& db) : 
        BaseSelectQueryCore<SelectEntityQueryCore<E>>(db) {

    }

    E GetElement(const Statement& statement) const {

        E entity{};

        auto columns = E::TableType::GetInstance().GetColumns();
        for (auto index : zaf::Range{ 0, columns.size() }) {
            columns[index]->GetValueFromStatement(statement, static_cast<int>(index), entity);
        }

        return entity;
    }
};

}