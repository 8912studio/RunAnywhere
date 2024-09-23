#pragma once

#include <sqlite3.h>
#include <zaf/base/error/base_runtime_error.h>

namespace ra::utility::sql {

class SQLError : public zaf::BaseRuntimeError {
public:
    SQLError(int code, const std::string& message) : BaseRuntimeError(message), code_(code) {

    }

    int Code() const {
        return code_;
    }

private:
    int code_{};
};


[[noreturn]]
void ThrowSQLError(int error_code, sqlite3* db_handle);

#define THROW_IF_SQL_ERROR(error_code, db_handle) \
if (error_code != SQLITE_OK) ra::utility::sql::ThrowSQLError(error_code, db_handle);

}