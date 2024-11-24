#include "utility/sql/table_schema.h"
#include <format>
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>

namespace ra::utility::sql {
namespace {

std::string GenerateColumnDefinitionSQL(const ColumnSchema& column) {

    std::string result = std::format(
        "{} {}",
        column.name, 
        DataTypeTraits::ToString(column.data_type));

    if (zaf::HasFlag(column.constraints, ColumnConstraints::PrimaryKey)) {
        result += " primary key";
    }

    if (zaf::HasFlag(column.constraints, ColumnConstraints::AutoIncrement)) {
        result += " autoincrement";
    }

    if (zaf::HasFlag(column.constraints, ColumnConstraints::NotNull)) {
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


std::string GeneratePrimaryKeyConstraint(const std::vector<std::string>& columns) {

    if (columns.empty()) {
        return {};
    }

    return std::format(", primary key ({})", zaf::JoinAsString(columns, ","));
}

}

std::string ToSQL(const TableSchema& table_schema) {

    return std::format(
        "create table if not exists {} ({} {})",
        table_schema.name,
        GenerateColumnDefinitionSQL(table_schema.columns),
        GeneratePrimaryKeyConstraint(table_schema.primary_key));
}

}