#pragma once

#include <map>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/create_round_task.h"
#include "module/ai/gpt/dialog/dialog_service_event_infos.h"
#include "module/ai/gpt/dialog/round.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

class DialogService : zaf::SubscriptionHost, zaf::NonCopyableNonMovable {
public:
    DialogService(
        std::shared_ptr<OpenAIClient> client, 
        std::shared_ptr<GPTStorage> storage);

    zaf::Observable<DialogList> FetchDialogs();
    std::shared_ptr<Dialog> CreateNewDialog();

    zaf::Observable<RoundList> FetchRoundsInDialog(DialogID dialog_id);
    
    std::shared_ptr<Round> CreateNewRound(
        std::shared_ptr<Dialog> dialog,
        std::vector<Message> messages);

    void DeleteRound(DialogID dialog_id, RoundID round_id);

    zaf::Observable<DialogCreatedInfo> DialogCreatedEvent() const {
        return dialog_created_event_.AsObservable();
    }

    zaf::Observable<DialogUpdatedInfo> DialogUpdatedEvent() const {
        return dialog_updated_event_.AsObservable();
    }

    zaf::Observable<DialogPersistedInfo> DialogPersistedEvent() const {
        return dialog_persisted_event_.AsObservable();
    }

    zaf::Observable<RoundCreatedInfo> RoundCreatedEvent() const {
        return round_created_event_.AsObservable();
    }

    zaf::Observable<RoundPersistedInfo> RoundPersistedEvent() const {
        return round_persisted_event_.AsObservable();
    }

private:
    zaf::Observable<ChatCompletion> CreateRoundAnswerFromEntity(const RoundEntity& entity);
    bool TryToDeleteCreatingRound(DialogID dialog_id, RoundID round_id);

private:
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    std::size_t new_dialog_transient_id_{ 1 };

    std::size_t new_round_transient_id_{ 1 };
    std::map<DialogID, std::shared_ptr<CreateRoundTask>> create_round_tasks_;

    zaf::Subject<DialogCreatedInfo> dialog_created_event_;
    zaf::Subject<DialogUpdatedInfo> dialog_updated_event_;
    zaf::Subject<DialogPersistedInfo> dialog_persisted_event_;
    zaf::Subject<RoundCreatedInfo> round_created_event_;
    zaf::Subject<RoundPersistedInfo> round_persisted_event_;
};

}