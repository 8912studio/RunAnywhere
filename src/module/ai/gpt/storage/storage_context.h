#pragma once

#include <filesystem>
#include <mutex>
#include <zaf/base/non_copyable.h>
#include "module/ai/gpt/storage/dialog_entity.h"
#include "module/ai/gpt/storage/round_entity.h"
#include "utility/sql/database.h"
#include "utility/sql/orm/data_set.h"

namespace ra::mod::ai::gpt {

class StorageContext : zaf::NonCopyableNonMovable {
public:
    explicit StorageContext(std::filesystem::path db_path);

    utility::sql::Database& DB();
    utility::sql::DataSet<DialogEntity>& DialogDataSet();
    utility::sql::DataSet<RoundEntity>& RoundDataSet();

private:
    void Initialize();

private:
    std::filesystem::path db_path_;

    std::optional<utility::sql::Database> db_;
    std::optional<utility::sql::DataSet<DialogEntity>> dialog_data_set_;
    std::optional<utility::sql::DataSet<RoundEntity>> round_data_set_;
    std::once_flag db_once_flag_;
};

}