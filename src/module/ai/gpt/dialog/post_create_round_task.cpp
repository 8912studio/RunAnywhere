#include "module/ai/gpt/dialog/post_create_round_task.h"
#include <zaf/base/string/encoding_conversion.h>

namespace ra::mod::ai::gpt {

PostCreateRoundTask::PostCreateRoundTask(
    std::shared_ptr<GPTStorage> storage,
    std::shared_ptr<PreCreateRoundTask> pre_task) 
    : 
    storage_(storage),
    pre_task_(std::move(pre_task)) {

}


void PostCreateRoundTask::Run() {

    ZAF_EXPECT(pre_task_);

    Subscriptions() += pre_task_->DialogUpdatedEvent().Subscribe(
        std::bind_front(&PostCreateRoundTask::OnDialogUpdated, this));

    Subscriptions() += pre_task_->RoundCreatedEvent().Subscribe(
        std::bind_front(&PostCreateRoundTask::OnRoundCreated, this));

    Subscriptions() += pre_task_->TaskFinishedEvent().Subscribe(
        std::bind_front(&PostCreateRoundTask::OnPreTaskFinished, this),
        [this](const std::exception_ptr&) {
            RaiseFinishEvent();
        });
}


void PostCreateRoundTask::OnDialogUpdated(const DialogUpdatedInfo& event_info) {

    const auto& entity = event_info.dialog->Entity();

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


void PostCreateRoundTask::OnRoundCreated(const RoundCreatedInfo& event_info) {

    auto round_entity = std::make_shared<RoundEntity>();
    round_entity->create_time = event_info.round->UpdatedTime();
    round_entity->update_time = event_info.round->UpdatedTime();
    round_entity->question = zaf::ToUTF8String(event_info.round->Question());

    Subscriptions() += dialog_saved_signal_.AsObservable()
        .FlatMap<std::shared_ptr<RoundEntity>>([this, round_entity](std::uint64_t dialog_id) {

            persisted_round_id_ = RoundPermanentID{ dialog_id };

            round_entity->dialog_id = dialog_id;

            return storage_->RoundStorage()->AddRound(*round_entity)
                .Map<std::shared_ptr<RoundEntity>>([round_entity](std::uint64_t round_id) {
                round_entity->id = round_id;
                return round_entity;
            });
        })
        .Subscribe(round_saved_signal_.AsObserver());
}


void PostCreateRoundTask::OnPreTaskFinished(const ChatResult& chat_result) {

    Subscriptions() += round_saved_signal_.AsObservable()
        .FlatMap<std::uint64_t>(
            [this, response = chat_result.Response()](const std::shared_ptr<RoundEntity>& entity) {

            entity->response = std::move(response);
            return storage_->RoundStorage()->UpdateRound(*entity);
        })
        .DoOnTerminated([this]() {
            RaiseFinishEvent();
        })
        .Subscribe();
}


void PostCreateRoundTask::RaiseFinishEvent() {

    RoundTaskFinishedInfo finish_event;

    auto dialog_transient_id = pre_task_->GetDialog()->ID().TransientID();
    if (dialog_transient_id && persisted_dialog_id_) {
        finish_event.dialog_persisted_info = DialogPersistedInfo{
            .transient_id = *dialog_transient_id,
            .permanent_id = *persisted_dialog_id_,
        };
    }

    if (persisted_round_id_) {
        finish_event.round_persisted_info = RoundPersistedInfo{
            .dialog_id = pre_task_->GetDialog()->ID(),
            .transient_id = *pre_task_->GetRound()->ID().TransientID(),
            .permanent_id = *persisted_round_id_,
        };
    }

    //The task instance may be destroyed after emitting OnNext, so we retain the observer to avoid
    //dangling pointer when emitting OnCompleted.
    auto observer = finished_event_.AsObserver();
    observer.OnNext(finish_event);
    observer.OnCompleted();
}

}