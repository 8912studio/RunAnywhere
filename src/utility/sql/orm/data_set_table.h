#pragma once

#include "utility/sql/database.h"
#include "utility/sql/orm/entity_meta.h"

namespace ra::utility::sql {

class DataSetTable {
public:
    static void InitializeTable(const AbstractEntityMeta& meta, Database& db);

private:
    static TableSchema ToTableSchema(const AbstractEntityMeta& meta);
};

}