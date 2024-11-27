#pragma once

#include "utility/sql/database.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/primary_key_traits.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

template<typename E>
class DataUpdater {
private:
    using TableType = Table<E>;

    static const TableType& Table() {
        return TableType::GetInstance();
    }

public:
    template<typename T = TableType, typename K = std::enable_if_t<HasPrimaryKeyV<T>>>
    static void Update(Database& db, const E& entity) {

        static const auto non_pk_columns = []() {
            auto pk_columns = Table().PrimaryKey.GetColumns();
            std::vector<const Column<E>*> result;
            for (auto each_column : Table().GetColumns()) {
                if (!zaf::Contain(pk_columns, each_column)) {
                    result.push_back(each_column);
                }
            }
            return result;
        }();

        static const auto sql = []() {
            return std::format("update {} set {} where {}",
                Table().GetName(),
                zaf::JoinAsString(non_pk_columns, ",", [](auto column) {
                    return std::format("{}=?", column->GetName());
                }),
                MakeKeyEquation(Table().PrimaryKey));
        }();

        auto statement = db.PrepareStatement(sql);
        BindEntityValuesToStatement<E>(statement, 1, non_pk_columns, entity);

        BindEntityValuesToStatement<E>(
            statement,
            static_cast<int>(1 + non_pk_columns.size()),
            Table().PrimaryKey.GetColumns(),
            entity);

        statement.Step();
    }
};

}