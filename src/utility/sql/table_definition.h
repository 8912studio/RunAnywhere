#pragma once

#include <string>
#include <vector>
#include "utility/sql/data_type.h"

namespace ra::utility::sql {

struct ColumnDefinition {
    std::string name;
    DataType data_type{ DataType::Unspecified };
};


struct TableDefinition {
    std::string name;
    std::vector<ColumnDefinition> columns;
};

}