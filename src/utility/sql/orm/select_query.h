#pragma once

#include <format>
#include "utility/sql/database.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/limit_query.h"
#include "utility/sql/orm/key.h"

namespace ra::utility::sql {

template<typename E, typename... Columns>
class SelectQuery {
private:
    using ThisQueryType = SelectQuery<E, Columns...>;

public:
    SelectQuery(Database& db, const Columns...& columns) : db_(db), columns_(columns...) {

    }

    std::string BuildSQL() const {

        return std::format(
            "select {} from {}", 
            JoinColumnNames(columns_.GetColumns()), 
            E::TableType::GetInstance()->GetName());
    }

    std::vector<Key<E, Columns...>::ValueType> Execute() const {

        std::vector<Key<E, Columns...>::ValueType> result;

        auto statement = db_.PrepareStatement(BuildSQL());
        while (statement.Step()) {


        }

        return result;
    }

    LimitQuery<ThisQueryType> Limit(std::size_t limit) {
        return LimitQuery<ThisQueryType>(*this, limit);
    }

private:
    Database& db_;
    Key<E, Columns...> columns_;
};

}