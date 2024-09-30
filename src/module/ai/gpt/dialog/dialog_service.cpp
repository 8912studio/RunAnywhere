#include "module/ai/gpt/dialog/dialog_service.h"
#include <zaf/base/container/utility/find.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>
#include "module/ai/gpt/network/response_parsing.h"

namespace ra::mod::ai::gpt {

DialogService::DialogService(
    std::shared_ptr<OpenAIClient> client,
    std::shared_ptr<GPTStorage> storage) 
    :
    client_(std::move(client)),
    storage_(std::move(storage)) {

}


zaf::Observable<DialogList> DialogService::FetchPermanentDialogs() {

    return storage_->DialogStorage()->FetchAllDialogs().Map<DialogList>(
        [this](const std::vector<DialogEntity>& dialog_entities) {

        DialogList dialogs;
        for (const auto& each_entity : dialog_entities) {

            DialogID dialog_id{ DialogPermanentID{ each_entity.id } };
            auto dialog = std::make_shared<Dialog>(dialog_id, each_entity);
            dialogs.push_back(dialog);
        }

        return dialogs;
    });
}


std::shared_ptr<Dialog> DialogService::CreateNewDialog() {

    DialogEntity entity;
    entity.create_time = std::time(nullptr);
    entity.update_time = entity.create_time;

    DialogID dialog_id{ DialogTransientID{ new_dialog_transient_id_++ } };
    auto new_dialog = std::make_shared<Dialog>(dialog_id, std::move(entity));
    return new_dialog;
}


zaf::Observable<RoundList> DialogService::FetchPermanentRoundsInDialog(
    DialogPermanentID dialog_permanent_id) {

    auto entities = storage_->RoundStorage()->FetchAllRoundsInDialog(dialog_permanent_id.Value());
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


zaf::Observable<ChatCompletion> DialogService::CreateRoundAnswerFromEntity(
    const RoundEntity& entity) {

    auto parsed = ParseChatCompletion(entity.response);
    if (parsed) {
        return zaf::rx::Just(*parsed);
    }
    return zaf::rx::Just(ChatCompletion{ Message{ L"" }, std::nullopt });
}


std::shared_ptr<CreateRoundTask> DialogService::CreateNewRound(
    std::shared_ptr<Dialog> dialog,
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

    task->Run();
    return task;
}


std::shared_ptr<CreateRoundTask> DialogService::GetCreateRoundTaskInDialog(DialogID dialog_id) {

    auto task = zaf::Find(create_round_tasks_, dialog_id);
    if (task) {
        return *task;
    }
    return nullptr;
}

}