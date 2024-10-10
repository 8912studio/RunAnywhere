#pragma once

#include <optional>
#include <zaf/base/none.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/dialog_service_event_infos.h"
#include "module/ai/gpt/dialog/id.h"
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

struct CreateRoundTaskFinishInfo {
    std::optional<DialogPersistedInfo> dialog_persisted_info;
    std::optional<RoundPersistedInfo> round_persisted_info;
};

class CreateRoundTask : zaf::SubscriptionHost {
public:
    CreateRoundTask(
        std::shared_ptr<OpenAIClient> client,
        std::shared_ptr<GPTStorage> storage);

    void Run(
        std::shared_ptr<Dialog> dialog,
        RoundTransientID round_transient_id,
        std::vector<Message> sent_messages);

    const std::shared_ptr<Dialog>& GetDialog() const {
        return dialog_;
    }

    const std::shared_ptr<Round>& GetRound() const {
        return round_;
    }

    std::optional<DialogPermanentID> GetDialogPermanentID() const;
    std::optional<RoundPermanentID> GetRoundPermanentID() const;

    zaf::Observable<DialogUpdatedInfo> DialogUpdatedEvent() const {
        return dialog_updated_event_.AsObservable();
    }

    zaf::Observable<RoundCreatedInfo> RoundCreatedEvent() const {
        return round_created_event_.AsObservable();
    }

    zaf::Observable<CreateRoundTaskFinishInfo> FinishEvent() const {
        return finish_event_.AsObservable();
    }

private:
    std::wstring GetQuestion() const;

    void UpdateDialog(const std::shared_ptr<Dialog>& dialog, std::time_t update_time);
    void SaveDialog();
    void CreateRound(RoundTransientID round_transient_id, std::time_t update_time);
    void SaveRound(std::shared_ptr<RoundEntity> round_entity);
    void CreateChat();
    void SaveResponse(const std::string& response);
    void RaiseFinishEvent();

private:
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    std::shared_ptr<Dialog> dialog_;
    std::vector<Message> sent_messages_;

    std::shared_ptr<Round> round_;
    std::optional<DialogPermanentID> persisted_dialog_id_;
    std::optional<RoundPermanentID> persisted_round_id_;

    zaf::ReplaySubject<std::uint64_t> dialog_saved_signal_;
    zaf::ReplaySubject<std::shared_ptr<RoundEntity>> round_saved_signal_;
    zaf::ReplaySubject<ChatCompletion> chat_completed_signal_;
    zaf::ReplaySubject<zaf::None> response_saved_signal_;

    zaf::ReplaySubject<DialogUpdatedInfo> dialog_updated_event_;
    zaf::ReplaySubject<RoundCreatedInfo> round_created_event_;
    zaf::ReplaySubject<CreateRoundTaskFinishInfo> finish_event_;
};

}