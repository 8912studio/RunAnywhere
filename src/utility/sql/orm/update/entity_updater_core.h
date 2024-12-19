#pragma once

#include <zaf/base/string/join.h>
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/update/primitive_updater_core.h"

namespace ra::utility::sql {

template<typename E>
class EntityUpdaterCore : public PrimitiveUpdaterCore<EntityUpdaterCore<E>> {
public:
    using EntityType = E;

public:
    EntityUpdaterCore(Database& db, const E& entity) : 
        PrimitiveUpdaterCore<EntityUpdaterCore<E>>(db),
        entity_(entity) {

    }

    int BindParameters(Statement& statement, int begin_index) const {

        auto columns = E::TableType::GetInstance().GetColumns();
        BindEntityValuesToStatement<E>(statement, begin_index, columns, entity_);

        return static_cast<int>(begin_index + columns.size());
    }

private:
    friend class PrimitiveUpdaterCore<EntityUpdaterCore<E>>;

    std::string BuildAssignmentSQL() const {
        auto columns = E::TableType::GetInstance().GetColumns();
        return zaf::JoinAsString(columns, ",", [](auto column) {
            return std::format("{}=?", column->GetName());
        });
    }

    const E& entity_;
};

}