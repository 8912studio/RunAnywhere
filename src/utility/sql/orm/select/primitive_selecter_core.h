#pragma once

#include "utility/sql/database.h"
#include "utility/sql/orm/data_set_helpers.h"

namespace ra::utility::sql {

template<typename Core>
class PrimitiveSelecterCore {
public:
    explicit PrimitiveSelecterCore(Database& db) : db_(db) {

    }

    std::string BuildSQL() const {

        static const std::string sql = [this]() {
            const auto& core = static_cast<const Core&>(*this);
            return std::format(
                "select {} from {}",
                JoinColumnNames(core.GetAbstractColumns()),
                Core::EntityType::TableType::GetInstance().GetName());
        }();

        return sql;
    }

    Statement PrepareStatement(std::string_view sql) const {
        return db_.PrepareStatement(sql);
    }

    int BindParameters(Statement& statement, int begin_index) const {
        return begin_index;
    }

private:
    Database& db_;
};

}