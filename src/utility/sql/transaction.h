#pragma once

#include <zaf/base/non_copyable.h>

namespace ra::utility::sql {

class Database;

class Transaction : zaf::NonCopyable {
public:
    explicit Transaction(Database& database);
    ~Transaction();

    Transaction(Transaction&& other) noexcept;
    Transaction& operator=(Transaction&& other) noexcept;

    void Commit();
    void Rollback();

private:
    void TryToRollback() noexcept;

private:
    Database* database_{};
};

}