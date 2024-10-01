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


void CreateRoundTask::Run() {

    ZAF_EXPECT(dialog_);
    ZAF_EXPECT(round_transient_id_.Value() != 0);
    ZAF_EXPECT(!sent_messages_.empty());

    CreateChat();
    CreateRound();
}


void CreateRoundTask::CreateChat() {

    Subscriptions() += client_->CreateChatCompletion(sent_messages_).Do(
        [this](const ChatResult& chat_result) {
            chat_response_ = chat_result.Response();
        })
        .Map<ChatCompletion>([](const ChatResult& chat_result) {
            return chat_result.ChatCompletion();
        })
        .Do(answer_event_.AsObserver())
        .DoOnTerminated([this]() {
            is_chat_finished_ = true;
            RunPostStage();
        })
        .Subscribe();
}


void CreateRoundTask::CreateRound() {

    auto update_time = std::time(nullptr);

    Subscriptions() += SaveDialog(update_time).FlatMap<std::shared_ptr<RoundEntity>>(
        [this, update_time](const std::shared_ptr<const DialogEntity>& dialog_entity) {
            return SaveRound(DialogPermanentID{ dialog_entity->id }, update_time);
        })
        .DoOnTerminated([this]() {
            is_round_created_ = true;
            RunPostStage();
        })
        .Subscribe();
}


zaf::Observable<std::shared_ptr<const DialogEntity>> CreateRoundTask::SaveDialog(
    std::time_t update_time) {

    auto dialog_entity = std::make_shared<DialogEntity>(dialog_->Entity());
    dialog_entity->update_time = update_time;

    if (dialog_entity->create_time == 0) {
        dialog_entity->create_time = update_time;
    }

    if (dialog_entity->subject.empty()) {
        dialog_entity->subject = zaf::ToUTF8String(GetQuestion().substr(0, 100));
    }

    auto storage_observable = [this, &dialog_entity]() {
        if (dialog_entity->id == 0) {

            return storage_->DialogStorage()->AddDialog(*dialog_entity).Do(
                [this, dialog_entity](std::uint64_t permanent_id) {

                    dialog_entity->id = permanent_id;
                    dialog_permanent_id_ = DialogPermanentID{ permanent_id };

                    dialog_saved_event_.AsObserver().OnNext(DialogSavedInfo{
                        *dialog_->ID().TransientID(),
                        DialogPermanentID{ permanent_id },
                    });
                    dialog_saved_event_.AsObserver().OnCompleted();
                });
        }
        else {
            return storage_->DialogStorage()->UpdateDialog(*dialog_entity);
        }
    }();

    return storage_observable.Map<std::shared_ptr<const DialogEntity>>(
        [this, dialog_entity](std::uint64_t permanent_id) {

            dialog_updated_event_.AsObserver().OnNext(DialogUpdatedInfo{
                std::make_shared<Dialog>(dialog_->ID(), *dialog_entity),
            });
            dialog_updated_event_.AsObserver().OnCompleted();

            return dialog_entity;
        });
}


zaf::Observable<std::shared_ptr<RoundEntity>> CreateRoundTask::SaveRound(
    DialogPermanentID permanent_id,
    std::time_t update_time) {

    auto round_entity = std::make_shared<RoundEntity>();
    round_entity->create_time = update_time;
    round_entity->update_time = update_time;
    round_entity->dialog_id = permanent_id.Value();
    round_entity->question = zaf::ToUTF8String(GetQuestion());

    return storage_->RoundStorage()->AddRound(*round_entity)
        .Map<std::shared_ptr<RoundEntity>>([this, round_entity](std::uint64_t permanent_id) {

            round_entity->id = permanent_id;
            new_round_entity_ = round_entity;

            round_saved_event_.AsObserver().OnNext(RoundSavedInfo{
                round_transient_id_,
                RoundPermanentID{ round_entity->id },
            });
            round_saved_event_.AsObserver().OnCompleted();
            return round_entity;
        });
}


void CreateRoundTask::RunPostStage() {

    if (!is_chat_finished_ || !is_round_created_) {
        return;
    }

    Subscriptions() += UpdateRoundInPostStage().DoOnTerminated([this]() {
        auto finish_observer = finish_event_.AsObserver();
        finish_observer.OnNext({});
        finish_observer.OnCompleted();
    })
    .Subscribe();
}


zaf::Observable<zaf::None> CreateRoundTask::UpdateRoundInPostStage() {

    if (!chat_response_ || !new_round_entity_) {
        return zaf::rx::Just(zaf::None{});
    }

    new_round_entity_->response = std::move(*chat_response_);

    return storage_->RoundStorage()->UpdateRound(*new_round_entity_).Map<zaf::None>(
        [](std::uint64_t) {
            return zaf::None{};
        });
}


std::optional<DialogPermanentID> CreateRoundTask::GetDialogPermanentID() const {

    if (auto permanent_id = dialog_->ID().PermanentID()) {
        return *permanent_id;
    }

    return dialog_permanent_id_;
}

}