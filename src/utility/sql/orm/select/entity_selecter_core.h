#pragma once

#include <zaf/base/range.h>
#include "utility/sql/orm/select/primitive_selecter_core.h"

namespace ra::utility::sql {

template<typename E>
class EntitySelecterCore : public PrimitiveSelecterCore<EntitySelecterCore<E>> {
public:
    using EntityType = E;
    using ResultElementType = E;

public:
    explicit EntitySelecterCore(Database& db) : PrimitiveSelecterCore<EntitySelecterCore<E>>(db) {

    }

    E GetElement(const Statement& statement) const {

        E entity{};

        auto columns = E::TableType::GetInstance().GetColumns();
        for (auto index : zaf::Range{ 0, columns.size() }) {
            columns[index]->GetValueFromStatement(statement, static_cast<int>(index), entity);
        }

        return entity;
    }

private:
    friend class PrimitiveSelecterCore<EntitySelecterCore<E>>;

    AbstractColumnsView GetAbstractColumns() const {
        return E::TableType::GetInstance().GetAbstractColumns();
    }
};

}