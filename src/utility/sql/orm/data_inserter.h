#pragma once

#include <format>
#include <zaf/base/range.h>
#include "utility/sql/database.h"
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

template<typename E>
class DataInserter {
private:
    using TableType = Table<E>;

    static const TableType& Table() {
        return TableType::GetInstance();
    }

public:
    static void Insert(const E& entity, Database& db) {
        InsertOrReplace(entity, true, db);
    }

    static void Replace(const E& entity, Database& db) {
        InsertOrReplace(entity, false, db);
    }

private:
    template<bool HasAutoincrementColumn>
    struct ValueBinder;

    template<>
    struct ValueBinder<false> {
        static void BindValues(Statement& statement, const E& entity) {
            BindEntityValuesToStatement(statement, 1, Table().GetColumns(), entity);
        }
    };

    template<>
    struct ValueBinder<true> {
        static void BindValues(Statement& statement, const E& entity) {

            const auto& table = Table();
            auto columns = table.GetColumns();
            auto autoincrement_column = table.PrimaryKey.GetColumns().front();

            for (auto index : zaf::Range{ 0, columns.size() }) {

                int parameter_index = static_cast<int>(index) + 1;

                auto each_column = columns[index];
                if (each_column == autoincrement_column) {
                    statement.BindParameter(parameter_index, std::nullopt);
                }
                else {
                    each_column->BindValueToStatement(statement, parameter_index, entity);
                }
            }
        }
    };

    template<typename T>
    struct ValueBinderSelector {
    private:
        template<typename K>
        static constexpr bool Test(typename K::PrimaryKeyType::AutoincrementTag*) {
            return true;
        }

        template<typename K>
        static constexpr bool Test(...) {
            return false;
        }

        static constexpr bool HasAutoincrement = Test<T>(nullptr);

    public:
        using type = ValueBinder<HasAutoincrement>;
    };

private:
    static void InsertOrReplace(const E& entity, bool insert, Database& db) {

        auto sql = GetSQL(insert);
        auto statement = db.PrepareStatement(sql);
        ValueBinderSelector<TableType>::type::BindValues(statement, entity);
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