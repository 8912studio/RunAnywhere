#include "module/ai/gpt/dialog/dialog_manager.h"
#include <zaf/creation.h>

namespace ra::mod::ai::gpt {

DialogManager::DialogManager(
    std::shared_ptr<OpenAIClient> client,
    std::shared_ptr<GPTStorage> storage)
    :
    client_(std::move(client)),
    storage_(std::move(storage)) {

}


std::shared_ptr<Dialog> gpt::DialogManager::CreateNewDialog() {

    storage_->FetchAllDialogs().Subscribe();

    return std::make_shared<Dialog>(new_dialog_number_++, client_);
}

}