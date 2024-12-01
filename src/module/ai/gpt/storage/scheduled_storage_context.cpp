#include "module/ai/gpt/storage/scheduled_storage_context.h"
#include <zaf/rx/creation.h>

namespace ra::mod::ai::gpt {

ScheduledStorageContext::ScheduledStorageContext(std::filesystem::path db_path) : 
    context_(std::move(db_path)) {

}


zaf::Observable<StorageContext*> ScheduledStorageContext::GetContextOnScheduler() {

    std::call_once(scheduler_once_flag_, [this]() {
        scheduler_ = zaf::Scheduler::CreateOnSingleThread();
    });

    return zaf::rx::Create<StorageContext*>(
        scheduler_,
        [this](zaf::Observer<StorageContext*> observer) {

        observer.OnNext(&context_);
        observer.OnCompleted();
    });
}

}