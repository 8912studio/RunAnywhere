#pragma once

#include <filesystem>
#include <mutex>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/scheduler.h>
#include "utility/sql/database.h"

namespace ra::mod::ai::gpt {

class StorageContext : zaf::NonCopyableNonMovable {
public:
    explicit StorageContext(std::filesystem::path db_path);

    template<typename T>
    zaf::Observable<T> Execute(const std::function<T(utility::sql::Database&)>& procedure) {
        return this->Database().Map<T>([procedure](utility::sql::Database* db) {
            return procedure(*db);
        }).ObserveOn(zaf::Scheduler::Main());
    }

private:
    zaf::Observable<utility::sql::Database*> Database();

private:
    std::filesystem::path db_path_;

    std::optional<utility::sql::Database> db_;
    std::once_flag db_once_flag_;

    std::shared_ptr<zaf::Scheduler> scheduler_;
    std::once_flag scheduler_once_flag_;
};

}