#include "utility/sql/orm/table_initializer.h"
#include <zaf/base/container/utility/sort.h>
#include "utility/sql/transaction.h"

namespace ra::utility::sql {

void TableInitializer::Initialize(const AbstractTable& table, Database& db) {

    auto table_info = db.GetTableInfo(table.GetName());
    if (!table_info) {
        db.CreateTable(ToTableSchema(table));
    }
    else {
        AlterTable(table, std::move(*table_info), db);
    }
}


TableSchema TableInitializer::ToTableSchema(const AbstractTable& table) {

    TableSchema table_schema;
    table_schema.name = table.GetName();

    for (auto each_field : table.GetAbstractColumns()) {

        ColumnSchema column_schema;
        column_schema.name = each_field->GetName();
        column_schema.data_type = each_field->GetDataType();

        table_schema.columns.push_back(std::move(column_schema));
    }

    auto primary_key = table.GetAbstractPrimaryKey();
    if (primary_key) {

        auto primary_key_columns = primary_key->GetAbstractColumns();
        if (primary_key_columns.size() == 1) {

            auto primary_column = primary_key_columns.front();
            for (auto& each_column : table_schema.columns) {

                if (each_column.name == primary_column->GetName()) {
                    each_column.constraints |= ColumnConstraints::PrimaryKey;
                    if (primary_key->IsAutoincrement()) {
                        each_column.constraints |= ColumnConstraints::AutoIncrement;
                    }
                    break;
                }
            }
        }
        else if (primary_key_columns.size() > 1) {

            for (auto each_field : primary_key_columns) {
                table_schema.primary_key.push_back(std::string{ each_field->GetName() });
            }
        }
    }

    return table_schema;
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
            existent_table_info.columns.begin(),
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
            "alter table {} add column {} {}",
            table.GetName(),
            each_column->GetName(),
            DataTypeTraits::ToString(each_column->GetDataType()));

        db.ExecuteSQL(sql);
    }

    transaction.Commit();
}

}