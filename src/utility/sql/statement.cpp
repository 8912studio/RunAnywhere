#include "utility/sql/statement.h"
#include "utility/sql/sql_error.h"

namespace ra::utility::sql {

Statement::Statement(sqlite3* database_handle, sqlite3_stmt* statement_handle) noexcept :
    database_handle_(database_handle),
    statement_handle_(statement_handle) {

}


Statement::~Statement() {
    Finalize();
}


Statement::Statement(Statement&& other) noexcept : 
    database_handle_(other.database_handle_),
    statement_handle_(other.statement_handle_) {

    other.database_handle_ = nullptr;
    other.statement_handle_ = nullptr;
}


Statement& Statement::operator=(Statement&& other) noexcept {
    if (this != &other) {
        Finalize();
        database_handle_ = other.database_handle_;
        statement_handle_ = other.statement_handle_;
        other.database_handle_ = nullptr;
        other.statement_handle_ = nullptr;
    }
    return *this;
}


void Statement::Finalize() noexcept {
    if (statement_handle_) {
        sqlite3_finalize(statement_handle_);
        statement_handle_ = nullptr;
    }
}


void Statement::BindParameter(int parameter_index, int value) {
    int error_code = sqlite3_bind_int(statement_handle_, parameter_index, value);
    THROW_IF_SQL_ERROR(error_code, database_handle_);
}


void Statement::BindParameter(int parameter_index, std::int64_t value) {
    int error_code = sqlite3_bind_int64(statement_handle_, parameter_index, value);
    THROW_IF_SQL_ERROR(error_code, database_handle_);
}


void Statement::BindParameter(int parameter_index, std::string_view value) {

    int error_code = sqlite3_bind_text(
        statement_handle_,
        parameter_index, 
        value.data(),
        static_cast<int>(value.length()),
        SQLITE_STATIC);

    THROW_IF_SQL_ERROR(error_code, database_handle_);
}


void Statement::ClearBindings() {
    int error_code = sqlite3_clear_bindings(statement_handle_);
    THROW_IF_SQL_ERROR(error_code, database_handle_);
}


bool Statement::Step() {

    int error_code = sqlite3_step(statement_handle_);
    if (error_code == SQLITE_DONE) {
        return true;
    }

    if (error_code == SQLITE_ROW) {
        return false;
    }

    ThrowSQLError(error_code, database_handle_);
}


void Statement::Reset() {
    int error_code = sqlite3_reset(statement_handle_);
    THROW_IF_SQL_ERROR(error_code, database_handle_);
}


int Statement::GetColumnInt(int column_index) noexcept {
    return sqlite3_column_int(statement_handle_, column_index);
}


std::int64_t Statement::GetColumnInt64(int column_index) noexcept {
    return sqlite3_column_int64(statement_handle_, column_index);
}

}