#include "module/ai/gpt/storage/storage_context.h"

namespace ra::mod::ai::gpt {

StorageContext::StorageContext(std::filesystem::path db_path) : db_path_(std::move(db_path)) {

}


void StorageContext::Initialize() {

    std::call_once(db_once_flag_, [this]() {

        std::filesystem::create_directories(db_path_.parent_path());
        db_ = utility::sql::Database::Open(db_path_);
        dialog_data_set_.emplace(*db_);
    });
}


utility::sql::Database& StorageContext::DB() {
    Initialize();
    return *db_;
}


utility::sql::DataSet<DialogEntity>& StorageContext::DialogDataSet() {
    Initialize();
    return *dialog_data_set_;
}

}