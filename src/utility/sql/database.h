#pragma once

#include <filesystem>
#include <sqlite3.h>
#include <zaf/base/non_copyable.h>
#include "utility/sql/statement.h"

namespace ra::utility::sql {

class Database : zaf::NonCopyable {
public:
    static Database Open(const std::filesystem::path& path);

public:
    explicit Database(sqlite3* handle) noexcept;
    ~Database();

    Database(Database&& other) noexcept;
    Database& operator=(Database&& other) noexcept;

    Statement PrepareStatement(const std::string& sql);

    sqlite3* Handle() const noexcept {
        return handle_;
    }

private:
    void Reset() noexcept;

private:
    sqlite3* handle_{};
};

}