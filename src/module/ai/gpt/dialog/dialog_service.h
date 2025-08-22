#pragma once

#include <map>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/subject/subject.h>
#include <zaf/rx/disposable_host.h>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/dialog_service_event_infos.h"
#include "module/ai/gpt/dialog/post_create_round_task.h"
#include "module/ai/gpt/dialog/post_create_round_task_queue.h"
#include "module/ai/gpt/dialog/pre_create_round_task.h"
#include "module/ai/gpt/dialog/round.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

class DialogService : zaf::rx::DisposableHost, zaf::NonCopyableNonMovable {
public:
    DialogService(
        std::shared_ptr<OpenAIClient> client, 
        std::shared_ptr<GPTStorage> storage);

    zaf::rx::Observable<DialogList> FetchDialogs();
    std::shared_ptr<Dialog> CreateNewDialog();
    void DeleteDialog(DialogID dialog_id);

    /**
    The result may emit twice: the first emission is the ongoing round; the second emission is the 
    rounds from storage.
    */
    zaf::rx::Observable<RoundList> FetchRoundsInDialog(DialogID dialog_id);
    
    std::shared_ptr<Round> CreateNewRound(
        std::shared_ptr<Dialog> dialog,
        std::wstring question,
        RoundList history_rounds);

    void DeleteRound(DialogID dialog_id, RoundID round_id);

    zaf::rx::Observable<DialogCreatedInfo> DialogCreatedEvent() const {
        return dialog_created_event_.AsObservable();
    }

    zaf::rx::Observable<DialogUpdatedInfo> DialogUpdatedEvent() const {
        return dialog_updated_event_.AsObservable();
    }

    zaf::rx::Observable<DialogPersistedInfo> DialogPersistedEvent() const {
        return dialog_persisted_event_.AsObservable();
    }

    zaf::rx::Observable<RoundCreatedInfo> RoundCreatedEvent() const {
        return round_created_event_.AsObservable();
    }

    zaf::rx::Observable<RoundPersistedInfo> RoundPersistedEvent() const {
        return round_persisted_event_.AsObservable();
    }

private:
    struct OngoingRoundInfo {
        std::vector<std::shared_ptr<PreCreateRoundTask>> pre_tasks;
        PostCreateRoundTaskQueue post_task_queue;
        bool is_deleted{};
    };

private:
    zaf::rx::Observable<RoundList> FetchRoundsFromStorage(
        DialogPermanentID dialog_id,
        const std::shared_ptr<OngoingRoundInfo>& ongoing_info);
    std::shared_ptr<Round> CreateRoundFromEntity(const RoundEntity& entity);
    zaf::rx::Observable<ChatCompletion> CreateRoundAnswerFromEntity(const RoundEntity& entity);

    std::shared_ptr<PreCreateRoundTask> CreatePreCreateRoundTask(
        const std::shared_ptr<Dialog>& dialog);

    OngoingRoundInfo* GetOngoingRoundInfo(DialogID dialog_id);

    bool TryToDeleteCreatingRound(DialogID dialog_id, RoundID round_id);

    void DeleteDialogFromStorage(DialogPermanentID permanent_id);

private:
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    std::size_t new_dialog_transient_id_{ 1 };

    std::size_t new_round_transient_id_{ 1 };
    std::map<DialogID, std::shared_ptr<OngoingRoundInfo>> ongoing_round_infos_;

    zaf::rx::Subject<DialogCreatedInfo> dialog_created_event_;
    zaf::rx::Subject<DialogUpdatedInfo> dialog_updated_event_;
    zaf::rx::Subject<DialogPersistedInfo> dialog_persisted_event_;
    zaf::rx::Subject<RoundCreatedInfo> round_created_event_;
    zaf::rx::Subject<RoundPersistedInfo> round_persisted_event_;
};

}