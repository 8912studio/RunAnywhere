#include "module/ai/gpt/dialog/dialog_manager.h"
#include <zaf/base/container/utility/find.h>
#include <zaf/base/error/contract_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/network/response_parsing.h"

namespace ra::mod::ai::gpt {

DialogManager::DialogManager(
    std::shared_ptr<OpenAIClient> client,
    std::shared_ptr<GPTStorage> storage)
    :
    client_(std::move(client)),
    storage_(std::move(storage)),
    dialog_data_source_(std::make_shared<gpt::DialogDataSource>()) {

}


void DialogManager::Initialize() {

    Subscriptions() += storage_->DialogStorage()->FetchAllDialogs().Subscribe(
        [this](const std::vector<DialogEntity>& dialog_entities) {
    
        std::vector<std::shared_ptr<DialogItemData>> dialogs;
        for (const auto& each_entity : dialog_entities) {

            auto dialog = std::make_shared<DialogItemData>(each_entity, 0);
            dialogs.push_back(dialog);
        }

        dialog_data_source_->AddDialogs(std::move(dialogs));
    });
}


std::shared_ptr<Dialog> DialogManager::CreateNewDialog() {

    DialogEntity entity;
    entity.create_time = std::time(nullptr);
    entity.update_time = entity.create_time;

    DialogTransientID transient_id{ new_dialog_transient_id_++ };
    auto new_dialog = std::make_shared<Dialog>(transient_id, std::move(entity));

    dialog_data_source_->AddDialog(new_dialog);
    return new_dialog;
}


zaf::Observable<RoundList> DialogManager::FetchPermanentRoundsInDialog(DialogID dialog_id) {

    std::uint64_t dialog_permanent_id{};
    if (auto transient_id = dialog_id.TransientID()) {
        auto permanent_id = zaf::Find(dialog_permanent_id_map_, *transient_id);
        if (permanent_id) {
            dialog_permanent_id = permanent_id->Value();
        }
    }
    else if (auto permanent_id = dialog_id.PermanentID()) {
        dialog_permanent_id = permanent_id->Value();
    }

    if (dialog_permanent_id == 0) {
        return zaf::rx::Just(RoundList{});
    }

    auto entities = storage_->RoundStorage()->FetchAllRoundsInDialog(dialog_permanent_id);
    return entities.Map<RoundList>([this](const std::vector<RoundEntity>& entities) {

        RoundList result;
        for (const auto& each_entity : entities) {

            auto round_model = std::make_shared<Round>(
                RoundID{ RoundPermanentID{ each_entity.id } },
                zaf::FromUTF8String(each_entity.question),
                CreateRoundAnswerFromEntity(each_entity));

            result.push_back(std::move(round_model));
        }
        return result;
    });
}


zaf::Observable<ChatCompletion> DialogManager::CreateRoundAnswerFromEntity(
    const RoundEntity& entity) {

    auto parsed = ParseChatCompletion(entity.response);
    if (parsed) {
        return zaf::rx::Just(*parsed);
    }
    return zaf::rx::Just(ChatCompletion{ Message{ L"" }, std::nullopt });
}


std::shared_ptr<CreateRoundTask> DialogManager::CreateNewRound(
    std::shared_ptr<const Dialog> dialog,
    std::vector<Message> messages) {

    RoundTransientID round_transient_id{ new_round_transient_id_++ };

    auto task = std::make_shared<CreateRoundTask>(client_, storage_);
    task->SetDialog(dialog);
    task->SetSentMessages(std::move(messages));
    task->SetRoundTransientID(round_transient_id);

    create_round_tasks_[dialog->ID()] = task;

    Subscriptions() += task->FinishEvent().Subscribe([this, dialog](zaf::None) {
        create_round_tasks_.erase(dialog->ID());
    });

    Subscriptions() += task->DialogSavedEvent().Subscribe([this](const DialogSavedInfo& info) {
        dialog_permanent_id_map_[info.transient_id] = info.permanent_id;
    });
    
    task->Run();
    return task;
}


std::shared_ptr<CreateRoundTask> DialogManager::GetCreateRoundTaskInDialog(DialogID dialog_id) {

    auto task = zaf::Find(create_round_tasks_, dialog_id);
    if (task) {
        return *task;
    }
    return nullptr;
}

}