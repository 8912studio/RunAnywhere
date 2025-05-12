#pragma once

#include <zaf/base/none.h>
#include <zaf/rx/observable.h>
#include "module/ai/gpt/storage/dialog_entity.h"
#include "module/ai/gpt/storage/scheduled_storage_context.h"

namespace ra::mod::ai::gpt {

class DialogStorage {
public:
    explicit DialogStorage(std::shared_ptr<ScheduledStorageContext> context);

    zaf::Observable<std::vector<DialogEntity>> FetchAllDialogs();
    zaf::Observable<std::uint64_t> AddDialog(const DialogEntity& dialog);
    zaf::Observable<std::uint64_t> UpdateDialog(const DialogEntity& dialog);
    zaf::Observable<zaf::None> DeleteDialog(std::uint64_t dialog_id);
    
private:
    std::shared_ptr<ScheduledStorageContext> context_;
};

}