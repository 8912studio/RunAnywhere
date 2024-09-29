#pragma once

#include <filesystem>
#include <memory>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include "module/ai/gpt/storage/dialog_entity.h"
#include "module/ai/gpt/storage/dialog_storage.h"
#include "module/ai/gpt/storage/round_storage.h"
#include "module/ai/gpt/storage/storage_context.h"
#include "utility/sql/database.h"

namespace ra::mod::ai::gpt {

class GPTStorage : zaf::NonCopyableNonMovable {
public:
    explicit GPTStorage(std::filesystem::path db_path);

    const std::shared_ptr<gpt::DialogStorage>& DialogStorage() const {
        return dialog_storage_;
    }

    const std::shared_ptr<gpt::RoundStorage>& RoundStorage() const {
        return round_storage_;
    }

private:
    std::shared_ptr<StorageContext> context_;
    std::shared_ptr<gpt::DialogStorage> dialog_storage_;
    std::shared_ptr<gpt::RoundStorage> round_storage_;
};

}