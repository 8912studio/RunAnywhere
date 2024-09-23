#pragma once

#include <sqlite3.h>

namespace ra::utility::sql {

enum class DataType {
    Unspecified = 0,
    Integer = SQLITE_INTEGER,
    Float = SQLITE_FLOAT,
    Text = SQLITE_TEXT,
    BLOB = SQLITE_BLOB,
};

}