#pragma once

#include <string>
#include <vector>
#include <zaf/base/flags_enum.h>
#include "utility/sql/data_type.h"

namespace ra::utility::sql {

enum class ColumnConstraints {
    None          = 0,
    PrimaryKey    = 1 << 0,
    AutoIncrement = 1 << 1,
    NotNull       = 1 << 2,
    Unique        = 1 << 3,
};

ZAF_ENABLE_FLAGS_ENUM(ColumnConstraints);


struct ColumnSchema {
    std::string name;
    DataType data_type{ DataType::Unspecified };
    ColumnConstraints constraints{};
};


struct TableSchema {
    std::string name;
    std::vector<ColumnSchema> columns;
};


std::string ToSQL(const TableSchema& table_schema);

}