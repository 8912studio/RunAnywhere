#include "module/ai/gpt/storage/gpt_storage.h"

using namespace ra::utility::sql;

namespace ra::mod::ai::gpt {

GPTStorage::GPTStorage(std::filesystem::path db_path) :
    context_(std::make_shared<StorageContext>(std::move(db_path))),
    dialog_storage_(std::make_shared<gpt::DialogStorage>(context_)) {

}

}