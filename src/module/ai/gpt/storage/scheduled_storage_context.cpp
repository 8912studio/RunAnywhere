#include "module/ai/gpt/storage/scheduled_storage_context.h"
#include <zaf/rx/scheduler/single_thread_scheduler.h>

namespace ra::mod::ai::gpt {

ScheduledStorageContext::ScheduledStorageContext(std::filesystem::path db_path) : 
    context_(std::move(db_path)) {

}


zaf::rx::Observable<StorageContext*> ScheduledStorageContext::GetContextOnScheduler() {

    std::call_once(scheduler_once_flag_, [this]() {
        scheduler_ = std::make_shared<zaf::rx::SingleThreadScheduler>();
    });

    return zaf::rx::Observable<StorageContext*>::CreateOn(
        scheduler_,
        [this](zaf::rx::Subscriber<StorageContext*> subscriber) {

        subscriber.OnNext(&context_);
        subscriber.OnCompleted();
    });
}

}