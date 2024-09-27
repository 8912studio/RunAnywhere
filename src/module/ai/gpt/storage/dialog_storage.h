#pragma once

#include "module/ai/gpt/storage/dialog_entity.h"
#include "module/ai/gpt/storage/storage_context.h"

namespace ra::mod::ai::gpt {

class DialogStorage {
public:
    explicit DialogStorage(std::shared_ptr<StorageContext> context);

    zaf::Observable<std::vector<DialogEntity>> FetchAllDialogs();
    zaf::Observable<DialogID> AddDialog(const DialogEntity& dialog);
    zaf::Observable<DialogID> UpdateDialog(const DialogEntity& dialog);

private:
    void InitializeDialogTable(utility::sql::Database& db);

private:
    std::shared_ptr<StorageContext> context_;
    std::once_flag dialog_table_once_flag_;
};

}