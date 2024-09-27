#include "module/ai/gpt/dialog/dialog_manager.h"
#include <zaf/creation.h>

namespace ra::mod::ai::gpt {

DialogManager::DialogManager(
    std::shared_ptr<OpenAIClient> client,
    std::shared_ptr<GPTStorage> storage)
    :
    client_(std::move(client)),
    storage_(std::move(storage)),
    data_source_(std::make_shared<DialogDataSource>()) {

}


void gpt::DialogManager::Initialize() {

    Subscriptions() += storage_->DialogStorage()->FetchAllDialogs().Subscribe(
        [this](const std::vector<DialogEntity>& dialog_entities) {
    
        std::vector<std::shared_ptr<Dialog>> dialogs;
        for (const auto& each_entity : dialog_entities) {

            auto dialog = std::make_shared<Dialog>(0, each_entity, client_, storage_);
            dialogs.push_back(dialog);
        }

        data_source_->AddDialogs(std::move(dialogs));
    });
}


std::shared_ptr<Dialog> gpt::DialogManager::CreateNewDialog() {

    DialogEntity entity;
    entity.create_time = std::time(nullptr);
    entity.update_time = entity.create_time;

    auto transient_id = new_dialog_transient_id_++;
    auto new_dialog = std::make_shared<Dialog>(transient_id, std::move(entity), client_, storage_);

    data_source_->AddDialog(new_dialog);
    return new_dialog;
}

}