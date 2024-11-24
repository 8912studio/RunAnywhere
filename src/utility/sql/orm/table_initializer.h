#pragma once

#include "utility/sql/database.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

class TableInitializer {
public:
    static void Initialize(const AbstractTable& table, Database& db);

private:
    static TableSchema ToTableSchema(const AbstractTable& table);

    static void AlterTable(
        const AbstractTable& table, 
        TableInfo&& existent_table_info,
        Database& db);

    static void AddNewColumns(
        const AbstractTable& table,
        const std::vector<const AbstractColumn*>& columns,
        Database& db);
};

}