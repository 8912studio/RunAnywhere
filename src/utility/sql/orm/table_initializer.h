#pragma once

#include "utility/sql/database.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

class TableInitializer {
public:
    static void Initialize(const AbstractTable& table, Database& db);

private:
    static void CreateTable(const AbstractTable& table, Database& db);

    static std::string GenerateColumnDefinitionsSQL(
        std::span<const AbstractColumn* const> columns, 
        const AbstractColumn* inline_pk_column);

    static std::string GeneratePrimaryKeyConstraintSQL(const AbstractPrimaryKey* primary_key);

    static void AlterTable(
        const AbstractTable& table, 
        TableInfo&& existent_table_info,
        Database& db);

    static void AddNewColumns(
        const AbstractTable& table,
        const std::vector<const AbstractColumn*>& columns,
        Database& db);

    static std::string ToSQL(const AbstractColumn& column, bool is_autoincrement);
};

}