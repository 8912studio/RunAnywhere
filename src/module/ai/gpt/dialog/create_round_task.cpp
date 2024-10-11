#include "module/ai/gpt/dialog/create_round_task.h"
#include <zaf/base/error/contract_error.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/rx/creation.h>

namespace ra::mod::ai::gpt {

CreateRoundTask::CreateRoundTask(
    std::shared_ptr<OpenAIClient> client,
    std::shared_ptr<GPTStorage> storage)
    :
    client_(std::move(client)),
    storage_(std::move(storage)) {

}


void CreateRoundTask::Run(
    std::shared_ptr<Dialog> dialog, 
    RoundTransientID round_transient_id,
    std::wstring question,
    RoundList history_rounds) {

    ZAF_EXPECT(dialog);
    ZAF_EXPECT(round_transient_id.Value() != 0);
    ZAF_EXPECT(!question.empty());

    question_ = std::move(question);
    history_rounds_ = std::move(history_rounds);

    auto current_time = std::time(nullptr);
    UpdateDialog(dialog, current_time);
    CreateRound(round_transient_id, current_time);
    CreateChat();
}


void CreateRoundTask::UpdateDialog(
    const std::shared_ptr<Dialog>& dialog, 
    std::time_t update_time) {

    DialogEntity dialog_entity{ dialog->Entity() };
    dialog_entity.update_time = update_time;

    if (dialog_entity.create_time == 0) {
        dialog_entity.create_time = update_time;
    }

    if (dialog_entity.subject.empty()) {
        dialog_entity.subject = zaf::ToUTF8String(question_.substr(0, 100));
    }

    dialog_ = std::make_shared<Dialog>(dialog->ID(), std::move(dialog_entity));
    dialog_updated_event_.AsObserver().OnNext(DialogUpdatedInfo{ .dialog = dialog_ });
    dialog_updated_event_.AsObserver().OnCompleted();

    SaveDialog();
}


void CreateRoundTask::SaveDialog() {

    const auto& entity = dialog_->Entity();

    if (entity.id == 0) {

        Subscriptions() += storage_->DialogStorage()->AddDialog(entity).Do(
            [this](std::uint64_t permanent_id) {

            persisted_dialog_id_ = DialogPermanentID{ permanent_id };
        })
            .Subscribe(dialog_saved_signal_.AsObserver());
    }
    else {
        Subscriptions() += storage_->DialogStorage()->UpdateDialog(entity).Subscribe(
            dialog_saved_signal_.AsObserver());
    }
}


void CreateRoundTask::CreateRound(RoundTransientID round_transient_id, std::time_t update_time) {

    round_ = std::make_shared<Round>(
        RoundID{ round_transient_id },
        question_,
        chat_completed_signal_.AsObservable());

    round_created_event_.AsObserver().OnNext(RoundCreatedInfo{ 
        .dialog_id = dialog_->ID(),
        .round = round_ 
    });
    round_created_event_.AsObserver().OnCompleted();

    auto entity = std::make_shared<RoundEntity>();
    entity->create_time = update_time;
    entity->update_time = update_time;
    entity->question = zaf::ToUTF8String(question_);
    SaveRound(std::move(entity));
}


void CreateRoundTask::SaveRound(std::shared_ptr<RoundEntity> round_entity) {

    Subscriptions() += dialog_saved_signal_.AsObservable()
        .FlatMap<std::shared_ptr<RoundEntity>>([this, round_entity](std::uint64_t dialog_id) {

        round_entity->dialog_id = dialog_id;

        return storage_->RoundStorage()->AddRound(*round_entity)
            .Map<std::shared_ptr<RoundEntity>>([round_entity](std::uint64_t round_id) {
            round_entity->id = round_id;
            return round_entity;
        });
    })
        .Subscribe(round_saved_signal_.AsObserver());
}


void CreateRoundTask::CreateChat() {

    auto messages = GenerateMessages();

    Subscriptions() += client_->CreateChatCompletion(messages)
        .Do([this](const ChatResult& chat_result) {
            SaveResponse(chat_result.Response());
        })
        .Map<ChatCompletion>([](const ChatResult& chat_result) {
            return chat_result.ChatCompletion();
        })
        .Do(chat_completed_signal_.AsObserver())
        .Subscribe();
}


std::vector<Message> CreateRoundTask::GenerateMessages() const {

    std::vector<Message> result;
    for (const auto& each_round : history_rounds_) {
        ZAF_EXPECT(each_round->State() == RoundState::Completed);
        result.push_back(Message{ each_round->Question() });
        result.push_back(each_round->Answer().Message());
    }

    result.push_back(Message{ question_ });
    return result;
}


void CreateRoundTask::SaveResponse(const std::string& response) {

    Subscriptions() += round_saved_signal_.AsObservable()
        .FlatMap<std::uint64_t>([this, response](std::shared_ptr<RoundEntity> entity) {

            entity->response = std::move(response);
            return storage_->RoundStorage()->UpdateRound(*entity);
        })
        .DoOnTerminated([this]() {
            RaiseFinishEvent();
        })
        .Subscribe();
}


void CreateRoundTask::RaiseFinishEvent() {

    CreateRoundTaskFinishInfo finish_event;

    auto dialog_transient_id = dialog_->ID().TransientID();
    if (dialog_transient_id && persisted_dialog_id_) {
        finish_event.dialog_persisted_info = DialogPersistedInfo {
            .transient_id = *dialog_transient_id,
            .permanent_id = *persisted_dialog_id_,
        };
    }

    if (persisted_round_id_) {
        finish_event.round_persisted_info = RoundPersistedInfo{
            .dialog_id = dialog_->ID(),
            .transient_id = *round_->ID().TransientID(),
            .permanent_id = *persisted_round_id_,
        };
    }

    //The task instance may be destroyed after emitting OnNext, so we retain the observer to avoid
    //dangling pointer when emitting OnCompleted.
    auto observer = finish_event_.AsObserver();
    observer.OnNext(finish_event);
    observer.OnCompleted();
}


std::optional<DialogPermanentID> CreateRoundTask::GetDialogPermanentID() const {
    if (auto permanent_id = dialog_->ID().PermanentID()) {
        return *permanent_id;
    }
    return persisted_dialog_id_;
}


std::optional<RoundPermanentID> CreateRoundTask::GetRoundPermanentID() const {
    return persisted_round_id_;
}

}