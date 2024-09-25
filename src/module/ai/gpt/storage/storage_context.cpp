#include "module/ai/gpt/storage/storage_context.h"
#include <zaf/rx/creation.h>

namespace ra::mod::ai::gpt {

StorageContext::StorageContext(std::filesystem::path db_path) : db_path_(std::move(db_path)) {

}


zaf::Observable<utility::sql::Database*> StorageContext::Database() {

    std::call_once(scheduler_once_flag_, [this]() {
        scheduler_ = zaf::Scheduler::CreateOnSingleThread();
    });

    return zaf::rx::Create<utility::sql::Database*>(
        scheduler_, 
        [this](zaf::Observer<utility::sql::Database*> observer) {
        
        std::call_once(db_once_flag_, [this]() {

            std::filesystem::create_directories(db_path_.parent_path());
            db_ = utility::sql::Database::Open(db_path_);
        });

        observer.OnNext(&*db_);
        observer.OnCompleted();
    });
}

}