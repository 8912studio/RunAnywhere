#pragma once

#include <string>
#include <vector>
#include "utility/sql/data_type.h"

namespace ra::utility::sql {

struct ColumnInfo {
    std::string name;
    DataType data_type{ DataType::Null };
    bool is_primary_key{};
    bool is_nullable{};
};

struct TableInfo {
    std::vector<ColumnInfo> columns;
};

}