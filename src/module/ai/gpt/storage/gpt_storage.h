#pragma once

#include <filesystem>
#include <memory>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include "module/ai/gpt/storage/dialog_entity.h"
#include "module/ai/gpt/storage/storage_context.h"
#include "utility/sql/database.h"

namespace ra::mod::ai::gpt {

class GPTStorage : zaf::NonCopyableNonMovable {
public:
    explicit GPTStorage(std::filesystem::path db_path);

    zaf::Observable<std::vector<DialogEntity>> FetchAllDialogs();
    zaf::Observable<DialogID> AddDialog(const DialogEntity& entity);

private:
    void InitializeDialogTable(utility::sql::Database& db);

private:
    std::unique_ptr<StorageContext> context_;

    std::once_flag dialog_table_once_flag_;
};

}