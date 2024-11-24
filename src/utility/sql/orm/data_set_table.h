#pragma once

#include "utility/sql/database.h"
#include "utility/sql/orm/table.h"

namespace ra::utility::sql {

class DataSetTable {
public:
    static void InitializeTable(const AbstractTable& meta, Database& db);

private:
    static TableSchema ToTableSchema(const AbstractTable& meta);
};

}