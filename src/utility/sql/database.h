#pragma once

#include <filesystem>
#include <sqlite3.h>
#include <zaf/base/non_copyable.h>
#include "utility/sql/statement.h"
#include "utility/sql/table_schema.h"

namespace ra::utility::sql {

class Database : zaf::NonCopyable {
public:
    static Database Open(const std::filesystem::path& path);

public:
    explicit Database(sqlite3* handle) noexcept;
    ~Database();

    Database(Database&& other) noexcept;
    Database& operator=(Database&& other) noexcept;

    void ExecuteSQL(std::string_view sql);

    Statement PrepareStatement(std::string_view sql);

    std::int64_t LastInsertRowID() const;

    void CreateTable(const TableSchema& table_schema);

    sqlite3* Handle() const noexcept {
        return handle_;
    }

private:
    void Reset() noexcept;

private:
    sqlite3* handle_{};
};

}