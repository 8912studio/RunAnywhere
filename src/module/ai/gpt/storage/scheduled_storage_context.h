#pragma once

#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/scheduler.h>
#include "module/ai/gpt/storage/storage_context.h"

namespace ra::mod::ai::gpt {

class ScheduledStorageContext : zaf::NonCopyableNonMovable {
public:
    explicit ScheduledStorageContext(std::filesystem::path db_path);

    template<typename T>
    zaf::Observable<T> Execute(const std::function<T(StorageContext&)>& procedure) {
        return GetContextOnScheduler().Map<T>([procedure](StorageContext* context) {
            return procedure(*context);
        }).ObserveOn(zaf::Scheduler::Main());
    }

private:
    zaf::Observable<StorageContext*> GetContextOnScheduler();

private:
    StorageContext context_;

    std::shared_ptr<zaf::Scheduler> scheduler_;
    std::once_flag scheduler_once_flag_;
};

}