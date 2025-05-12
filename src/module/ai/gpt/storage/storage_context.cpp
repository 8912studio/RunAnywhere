#include "module/ai/gpt/storage/storage_context.h"

namespace ra::mod::ai::gpt {

StorageContext::StorageContext(std::filesystem::path db_path) : db_path_(std::move(db_path)) {

}


void StorageContext::Initialize() {

    std::call_once(db_once_flag_, [this]() {

        std::filesystem::create_directories(db_path_.parent_path());
        db_ = std::make_shared<sqt::Database>(sqt::Database::Open(db_path_));
        dialog_data_context_.emplace(db_);
        round_data_context_.emplace(db_);
    });
}


sqt::Database& StorageContext::DB() {
    Initialize();
    return *db_;
}


sqt::DataContext<DialogEntity>& StorageContext::DialogDataContext() {
    Initialize();
    return *dialog_data_context_;
}


sqt::DataContext<RoundEntity>& StorageContext::RoundDataContext() {
    Initialize();
    return *round_data_context_;
}

}