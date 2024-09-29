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


std::shared_ptr<DialogItemData> DialogManager::CreateNewDialog() {

    DialogEntity entity;
    entity.create_time = std::time(nullptr);
    entity.update_time = entity.create_time;

    auto transient_id = new_dialog_transient_id_++;
    auto new_dialog = std::make_shared<DialogItemData>(std::move(entity), transient_id);

    dialog_data_source_->AddDialog(new_dialog);
    return new_dialog;
}


zaf::Observable<RoundModelList> DialogManager::FetchAllRoundsInDialog(DialogID dialog_id) {

    auto entities = storage_->RoundStorage()->FetchAllRoundsInDialog(dialog_id);
    return entities.Map<RoundModelList>([this](const std::vector<RoundEntity>& entities) {

        RoundModelList result;
        for (const auto& each_entity : entities) {

            auto round_model = std::make_shared<RoundModel>(
                each_entity.ID(),
                zaf::FromUTF8String(each_entity.question),
                CreateRoundAnswerFromEntity(each_entity));

            result.push_back(std::move(round_model));
        }
        return result;
    });
}


zaf::Observable<ChatCompletion> DialogManager::CreateRoundAnswerFromEntity(
    const RoundEntity& entity) {

    auto ongoing_round_info = zaf::Find(ongoing_rounds_, entity.dialog_id);
    if (ongoing_round_info) {
        if (ongoing_round_info->answer) {
            return *ongoing_round_info->answer;
        }
    }

    auto parsed = ParseChatCompletion(entity.response);
    if (parsed) {
        return zaf::rx::Just(*parsed);
    }
    return zaf::rx::Just(ChatCompletion{ Message{ L"" }, std::nullopt });
}


std::shared_ptr<RoundModel> DialogManager::CreateNewRound(
    std::shared_ptr<DialogItemData> dialog,
    std::vector<Message> messages) {

    ZAF_EXPECT(dialog);
    ZAF_EXPECT(!messages.empty());

    auto transient_id = new_round_transient_id_++;
    auto question = messages.back().Content();
    auto answer = CreateRoundAnswer(dialog, std::move(messages));
    auto round_model = std::make_shared<RoundModel>(
        RoundID{ 0, transient_id },
        std::move(question), 
        answer);

    auto& ongoing_round_info = ongoing_rounds_[dialog->Entity().id];
    ongoing_round_info.transient_id = transient_id;
    ongoing_round_info.answer = answer;

    return round_model;
}


zaf::Observable<ChatCompletion> DialogManager::CreateRoundAnswer(
    std::shared_ptr<DialogItemData> dialog,
    std::vector<Message> messages) {

    auto new_round_entity = std::make_shared<RoundEntity>();
    new_round_entity->dialog_id = dialog->Entity().id;
    new_round_entity->create_time = std::time(nullptr);
    new_round_entity->update_time = new_round_entity->create_time;
    new_round_entity->question = zaf::ToUTF8String(messages.back().Content());

    return storage_->RoundStorage()->AddRound(*new_round_entity).FlatMap<ChatResult>(
        [this, new_round_entity, messages = std::move(messages)](RoundPermanentID round_id) {

        new_round_entity->permanent_id = round_id;
        return client_->CreateChatCompletion(messages);
    })
    .Map<ChatCompletion>([this, new_round_entity](const ChatResult& chat_result) {

        new_round_entity->update_time = std::time(nullptr);
        new_round_entity->response = chat_result.Response();

        Subscriptions() += storage_->RoundStorage()->UpdateRound(*new_round_entity).Subscribe(
            [](RoundPermanentID) {},
            [this, new_round_entity]() {
                ongoing_rounds_.erase(new_round_entity->dialog_id);
            }
        );
        return chat_result.ChatCompletion();
    });
}

}