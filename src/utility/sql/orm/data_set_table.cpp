#include "utility/sql/orm/data_set_table.h"

namespace ra::utility::sql {

void DataSetTable::InitializeTable(const AbstractTable& meta, Database& db) {

    auto table_info = db.GetTableInfo(meta.GetName());
    if (!table_info) {
        db.CreateTable(ToTableSchema(meta));
    }
    else {

    }
}


TableSchema DataSetTable::ToTableSchema(const AbstractTable& meta) {

    TableSchema table_schema;
    table_schema.name = meta.GetName();

    for (auto each_field : meta.GetAllAbstractColumns()) {

        ColumnSchema column_schema;
        column_schema.name = each_field->GetName();
        column_schema.data_type = each_field->GetDataType();

        table_schema.columns.push_back(std::move(column_schema));
    }

    auto primary_key = meta.GetAbstractPrimaryKey();
    if (primary_key) {

        auto primary_key_fields = primary_key->GetAbstractColumns();
        if (primary_key_fields.size() == 1) {

            auto primary_field = primary_key_fields.front();
            for (auto& each_column : table_schema.columns) {
                if (each_column.name == primary_field->GetName()) {
                    each_column.constraints |= ColumnConstraints::PrimaryKey;
                    break;
                }
            }
        }
        else if (primary_key_fields.size() > 1) {

            for (auto each_field : primary_key_fields) {
                table_schema.primary_key.push_back(std::string{ each_field->GetName() });
            }
        }
    }

    return table_schema;
}

}