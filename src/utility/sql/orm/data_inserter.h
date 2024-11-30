#pragma once

#include <format>
#include <zaf/base/range.h>
#include "utility/sql/database.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/primary_key_traits.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

template<typename E>
class DataInserter {
private:
    using TableType = typename E::TableType;

    static const TableType& Table() {
        return TableType::GetInstance();
    }

public:
    static void Insert(Database& db, const E& entity) {
        InsertOrReplace(db, entity, true);
    }

    static void Replace(Database& db, const E& entity) {
        InsertOrReplace(db, entity, false);
    }

    template<typename T = TableType>
    static std::enable_if_t<HasAutoincrementPrimaryKeyV<T>, typename T::PrimaryKeyType::ValueType> 
        InsertWithAutoincrement(Database& db, const E& entity) {

        InnerExecute(db, true, [&entity](Statement& statement) {
        
            const auto& table = Table();
            auto columns = table.GetColumns();
            auto autoincrement_column = table.PrimaryKey.GetColumns().front();

            for (auto index : zaf::Range{ 0, columns.size() }) {

                int parameter_index = static_cast<int>(index) + 1;

                auto each_column = columns[index];
                if (each_column == autoincrement_column) {
                    //Bind null value to the primary key to make it auto increase.
                    statement.BindParameter(parameter_index, std::nullopt);
                }
                else {
                    each_column->BindValueToStatement(statement, parameter_index, entity);
                }
            }
        });

        return static_cast<typename TableType::PrimaryKeyType::ValueType>(db.LastInsertRowID());
    }

private:
    static void InsertOrReplace(Database& db, const E& entity, bool insert) {
        InnerExecute(db, insert, [&entity](Statement& statement) {
            BindEntityValuesToStatement(statement, 1, Table().GetColumns(), entity);
        });
    }


    static void InnerExecute(
        Database& db,
        bool insert,
        const std::function<void(Statement&)>& value_binder) {

        auto sql = GetSQL(insert);
        auto statement = db.PrepareStatement(sql);
        value_binder(statement);
        statement.Step();
    }

    static std::string_view GetSQL(bool insert) {
        if (insert) {
            static auto sql = GenerateSQL(true);
            return sql;
        }
        else {
            static auto sql = GenerateSQL(false);
            return sql;
        }
    }

    static std::string GenerateSQL(bool insert) {
        std::string_view verb = insert ? "insert" : "replace";
        return std::format("{} into {} ({}) values ({})",
            verb,
            Table().GetName(),
            JoinColumnNames(Table().GetAbstractColumns()),
            JoinPlaceholders(Table().GetColumns().size()));
    }

private:
    DataInserter() = delete;
};

}