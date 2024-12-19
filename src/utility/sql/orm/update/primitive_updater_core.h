#pragma once

#include "utility/sql/database.h"
#include "utility/sql/orm/data_set_helpers.h"

namespace ra::utility::sql {

template<typename Core>
class PrimitiveUpdaterCore {
public:
    explicit PrimitiveUpdaterCore(Database& db) : db_(db) {

    }

    Database& GetDB() const {
        return db_;
    }

    std::string BuildSQL() const {

        static const std::string sql = [this]() {

            const auto& core = static_cast<const Core&>(*this);
            return std::format(
                "update {} set {}",
                Core::EntityType::TableType::GetInstance().GetName(),
                core.BuildAssignmentSQL());
        }();

        return sql;
    }

private:
    Database& db_;
};

}