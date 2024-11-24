#pragma once

#include <string_view>
#include <sqlite3.h>

namespace ra::utility::sql {

enum class DataType {
    Unspecified = 0,
    Integer = SQLITE_INTEGER,
    Float = SQLITE_FLOAT,
    Text = SQLITE_TEXT,
    BLOB = SQLITE_BLOB,
};


class DataTypeTraits {
public:
    static std::string_view ToString(DataType data_type);
    static DataType FromString(std::string_view string);

private:
    DataTypeTraits() = delete;
};

}