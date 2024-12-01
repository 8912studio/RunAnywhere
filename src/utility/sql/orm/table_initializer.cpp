#include "utility/sql/orm/table_initializer.h"
#include <format>
#include <zaf/base/container/utility/sort.h>
#include <zaf/base/string/join.h>
#include "utility/sql/orm/data_set_helpers.h"
#include "utility/sql/transaction.h"

namespace ra::utility::sql {

void TableInitializer::Initialize(const AbstractTable& table, Database& db) {

    auto table_info = db.GetTableInfo(table.GetName());
    if (!table_info) {
        CreateTable(table, db);
    }
    else {
        AlterTable(table, std::move(*table_info), db);
    }
}


void TableInitializer::CreateTable(const AbstractTable& table, Database& db) {

    const AbstractColumn* inline_pk_column{};
    auto pk = table.GetAbstractPrimaryKey();
    if (pk && pk->IsAutoincrement()) {
        inline_pk_column = pk->GetAbstractColumns().front();
    }

    auto sql = std::format(
        "create table if not exists {} ({} {})",
        table.GetName(),
        GenerateColumnDefinitionsSQL(table.GetAbstractColumns(), inline_pk_column),
        inline_pk_column ? 
            std::string{} : 
            GeneratePrimaryKeyConstraintSQL(table.GetAbstractPrimaryKey()));

    db.ExecuteSQL(sql);
}


std::string TableInitializer::GenerateColumnDefinitionsSQL(
    std::span<const AbstractColumn* const> columns, 
    const AbstractColumn* inline_pk_column) {

    return zaf::JoinAsString(columns, ",", [inline_pk_column](auto column) {
        return ToSQL(*column, column == inline_pk_column);
    });
}


std::string TableInitializer::GeneratePrimaryKeyConstraintSQL(
    const AbstractPrimaryKey* primary_key) {

    if (!primary_key) {
        return {};
    }

    return std::format(", primary key ({})", JoinColumnNames(primary_key->GetAbstractColumns()));
}


void TableInitializer::AlterTable(
    const AbstractTable& table, 
    TableInfo&& existent_table_info,
    Database& db) {

    zaf::Sort(existent_table_info.columns, [](const auto& column1, const auto& column2) {
        return column1.name < column2.name;
    });

    std::vector<const AbstractColumn*> inexistent_columns;
    for (auto each_column : table.GetAbstractColumns()) {

        auto iterator = std::lower_bound(
            existent_table_info.columns.begin(),
            existent_table_info.columns.end(),
            each_column,
            [](const ColumnInfo& existent_column, const AbstractColumn* abstract_column) {
                return existent_column.name < abstract_column->GetName();
            });

        if (iterator == existent_table_info.columns.end() ||
            iterator->name != each_column->GetName()) {
            inexistent_columns.push_back(each_column);
        }
    }

    if (!inexistent_columns.empty()) {
        AddNewColumns(table, inexistent_columns, db);
    }
}


void TableInitializer::AddNewColumns(
    const AbstractTable& table,
    const std::vector<const AbstractColumn*>& columns,
    Database& db) {

    Transaction transaction{ db };

    for (auto each_column : columns) {

        std::string sql = std::format(
            "alter table {} add column {}",
            table.GetName(),
            ToSQL(*each_column, false));

        db.ExecuteSQL(sql);
    }

    transaction.Commit();
}


std::string TableInitializer::ToSQL(const AbstractColumn& column, bool is_autoincrement) {

    std::string result = std::format(
        "{} {}",
        column.GetName(),
        DataTypeTraits::ToString(column.GetDataType()));

    if (!column.IsNullable()) {
        result += " not null";
    }

    if (is_autoincrement) {
        result += " primary key autoincrement";
    }

    return result;
}

}