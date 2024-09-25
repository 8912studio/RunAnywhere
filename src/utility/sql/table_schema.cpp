#include "utility/sql/table_schema.h"
#include <format>
#include <zaf/base/range.h>

namespace ra::utility::sql {
namespace {

std::string_view DataTypeToString(DataType data_type) {
    switch (data_type) {
    case DataType::Integer:
        return "integer";
    case DataType::Float:
        return "float";
    case DataType::Text:
        return "text";
    case DataType::BLOB:
        return "blob";
    case DataType::Unspecified:
    default:
        return "";
    }
}


std::string GenerateColumnDefinitionSQL(const ColumnSchema& column) {

    std::string result = std::format("{} {}", column.name, DataTypeToString(column.data_type));

    if (zaf::HasFlag(column.constraints, ColumnConstraints::PrimaryKey)) {
        result += " primary key";
    }

    if (zaf::HasFlag(column.constraints, ColumnConstraints::AutoIncrement)) {
        result += " autoincrement";
    }

    if (zaf::HasFlag(column.constraints, ColumnConstraints::None)) {
        result += " not null";
    }

    if (zaf::HasFlag(column.constraints, ColumnConstraints::Unique)) {
        result += " unique";
    }

    return result;
}


std::string GenerateColumnDefinitionSQL(const std::vector<ColumnSchema>& columns) {

    std::string result;
    for (auto index : zaf::Range(0, columns.size())) {

        if (index != 0) {
            result += ',';
        }

        result += GenerateColumnDefinitionSQL(columns[index]);
    }
    return result;
}

}

std::string ToSQL(const TableSchema& table_schema) {

    return std::format(
        "create table if not exists {} ({});",
        table_schema.name,
        GenerateColumnDefinitionSQL(table_schema.columns));
}

}