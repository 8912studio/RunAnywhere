#pragma once

#include <filesystem>
#include <mutex>
#include <sqt/orm/data_context.h>
#include <zaf/base/non_copyable.h>
#include "module/ai/gpt/storage/dialog_entity.h"
#include "module/ai/gpt/storage/round_entity.h"

namespace ra::mod::ai::gpt {

class StorageContext : zaf::NonCopyableNonMovable {
public:
    explicit StorageContext(std::filesystem::path db_path);

    sqt::Database& DB();
    sqt::DataContext<DialogEntity>& DialogDataContext();
    sqt::DataContext<RoundEntity>& RoundDataContext();

private:
    void Initialize();

private:
    std::filesystem::path db_path_;

    std::shared_ptr<sqt::Database> db_;
    std::optional<sqt::DataContext<DialogEntity>> dialog_data_context_;
    std::optional<sqt::DataContext<RoundEntity>> round_data_context_;
    std::once_flag db_once_flag_;
};

}