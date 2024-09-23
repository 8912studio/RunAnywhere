#include "utility/sql/sql_error.h"

namespace ra::utility::sql {

void ThrowSQLError(int error_code, sqlite3* db_handle) {

    std::string error_message;
    if (db_handle) {
        auto temp_error_message = sqlite3_errmsg(db_handle);
        if (temp_error_message) {
            error_message = temp_error_message;
        }
    }

    throw SQLError{ error_code, error_message };
}

}