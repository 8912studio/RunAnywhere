#include "utility/sql/database.h"
#include <zaf/base/string/encoding_conversion.h>
#include "utility/sql/sql_error.h"

namespace ra::utility::sql {

Database Database::Open(const std::filesystem::path& path) {

    sqlite3* handle{};

    auto utf8_path = path.u8string();
    int error_code = sqlite3_open_v2(
        reinterpret_cast<const char*>(utf8_path.c_str()), 
        &handle,
        SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, 
        nullptr);

    Database database{ handle };
    THROW_IF_SQL_ERROR(error_code, handle);
    return database;
}


Database::Database(sqlite3* handle) noexcept : handle_(handle) {

}


Database::~Database() {
    Reset();
}


Database::Database(Database&& other) noexcept : handle_(other.handle_) {
    other.handle_ = nullptr;
}


Database& Database::operator=(Database&& other) noexcept {

    if (this != &other) {
        Reset();
        handle_ = other.handle_;
        other.handle_ = nullptr;
    }
    return *this;
}


void Database::Reset() noexcept {

    if (handle_) {
        sqlite3_close_v2(handle_);
        handle_ = nullptr;
    }
}


Statement Database::PrepareStatement(const std::string& sql) {

    sqlite3_stmt* statement_handle{};
    int error_code = sqlite3_prepare_v2(
        handle_, 
        sql.c_str(),
        static_cast<int>(sql.length() + 1),
        &statement_handle, 
        nullptr);

    THROW_IF_SQL_ERROR(error_code, handle_);

    return Statement{ handle_, statement_handle };
}

}