#pragma once

#include <string>
#include <vector>
#include "utility/sql/data_type.h"

namespace ra::utility::sql {

struct ColumnInfo {
    std::string name;
    DataType data_type{ DataType::Unspecified };
};

struct TableInfo {
    std::vector<ColumnInfo> columns;
};

}