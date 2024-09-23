#pragma once

#include <cstdint>
#include <string_view>
#include <sqlite3.h>
#include <zaf/base/non_copyable.h>

namespace ra::utility::sql {

class Statement : zaf::NonCopyable {
public:
    Statement(sqlite3* database_handle, sqlite3_stmt* statement_handle) noexcept;
    ~Statement();

    Statement(Statement&& other) noexcept;
    Statement& operator=(Statement&& other) noexcept;

    void BindParameter(int parameter_index, int value);
    void BindParameter(int parameter_index, std::int64_t value);
    void BindParameter(int parameter_index, std::string_view value);
    void ClearBindings();

    bool Step();
    void Reset();

    int GetColumnInt(int column_index) noexcept;
    std::int64_t GetColumnInt64(int column_index) noexcept;

    sqlite3_stmt* Handle() const noexcept {
        return statement_handle_;
    }

private:
    void Finalize() noexcept;

private:
    sqlite3* database_handle_{};
    sqlite3_stmt* statement_handle_{};
};

}