#pragma once

#include "module/ai/gpt/dialog/pre_create_round_task.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

struct RoundTaskFinishedInfo {
    std::optional<DialogPersistedInfo> dialog_persisted_info;
    std::optional<RoundPersistedInfo> round_persisted_info;
};

class PostCreateRoundTask : zaf::rx::DisposableHost {
public:
    PostCreateRoundTask(
        std::shared_ptr<GPTStorage> storage,
        std::shared_ptr<PreCreateRoundTask> pre_task);

    void Run();

    const std::shared_ptr<PreCreateRoundTask>& PreTask() const {
        return pre_task_;
    }

    std::optional<RoundPermanentID> GetPersistedRoundID() const {
        return persisted_round_id_;
    }

    zaf::rx::Observable<RoundTaskFinishedInfo> FinishedEvent() const {
        return finished_event_.AsObservable();
    }

private:
    void OnDialogUpdated(const DialogUpdatedInfo& event_info);
    void OnRoundCreated(const RoundCreatedInfo& event_info);
    void OnPreTaskFinished(const ChatResult& chat_result);
    void RaiseFinishEvent();

private:
    std::shared_ptr<GPTStorage> storage_;
    std::shared_ptr<PreCreateRoundTask> pre_task_;

    std::optional<DialogPermanentID> persisted_dialog_id_;
    std::optional<RoundPermanentID> persisted_round_id_;

    zaf::rx::ReplaySubject<std::uint64_t> dialog_saved_signal_;
    zaf::rx::ReplaySubject<std::shared_ptr<RoundEntity>> round_saved_signal_;
    zaf::rx::ReplaySubject<RoundTaskFinishedInfo> finished_event_;
};

}