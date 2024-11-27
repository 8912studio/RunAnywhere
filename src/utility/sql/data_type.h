#pragma once

#include <string_view>
#include <sqlite3.h>

namespace ra::utility::sql {

enum class DataType {
    Integer = SQLITE_INTEGER,
    Float = SQLITE_FLOAT,
    Text = SQLITE_TEXT,
    BLOB = SQLITE_BLOB,
    Null = SQLITE_NULL,
};


class DataTypeTraits {
public:
    static std::string_view ToString(DataType data_type);
    static DataType FromString(std::string_view string);

private:
    DataTypeTraits() = delete;
};

}