#pragma once

#include <optional>
#include <zaf/base/none.h>
#include <zaf/rx/subject.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/id.h"
#include "module/ai/gpt/dialog/list/dialog_item_data.h"
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

struct DialogSavedInfo {
    DialogTransientID transient_id;
    DialogPermanentID permanent_id;
};

struct RoundSavedInfo {
    RoundTransientID transient_id;
    RoundPermanentID permanent_id;
};

class CreateRoundTask : zaf::SubscriptionHost {
public:
    CreateRoundTask(
        std::shared_ptr<OpenAIClient> client,
        std::shared_ptr<GPTStorage> storage);

    void SetRoundTransientID(RoundTransientID round_transient_id) {
        round_transient_id_ = round_transient_id;
    }

    void SetDialog(std::shared_ptr<const Dialog> dialog) {
        dialog_ = std::move(dialog);
    }

    void SetSentMessages(std::vector<Message> messages) {
        sent_messages_ = std::move(messages);
    }

    void Run();

    RoundTransientID GetRoundTransientID() const {
        return round_transient_id_;
    }

    std::optional<RoundPermanentID> GetRoundPermanentID() const {
        if (new_round_entity_) {
            return RoundPermanentID{ new_round_entity_->id };
        }
        return std::nullopt;
    }

    zaf::Observable<ChatCompletion> AnswerEvent() const {
        return answer_event_.AsObservable();
    }

    zaf::Observable<DialogSavedInfo> DialogSavedEvent() const {
        return dialog_saved_event_.AsObservable();
    }

    zaf::Observable<RoundSavedInfo> RoundSavedEvent() const {
        return round_saved_event_.AsObservable();
    }

    zaf::Observable<zaf::None> FinishEvent() const {
        return finish_event_.AsObservable();
    }

private:
    void CreateChat();
    void CreateRound();
    zaf::Observable<std::shared_ptr<const DialogEntity>> SaveDialog(std::time_t update_time);
    zaf::Observable<std::shared_ptr<RoundEntity>> SaveRound(
        DialogPermanentID permanent_id, 
        std::time_t update_time);

    void RunPostStage();
    zaf::Observable<zaf::None> UpdateRoundInPostStage();

private:
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    RoundTransientID round_transient_id_;
    std::shared_ptr<const Dialog> dialog_;
    std::vector<Message> sent_messages_;

    bool is_chat_finished_{};
    bool is_round_created_{};
    std::optional<std::string> chat_response_;
    std::shared_ptr<RoundEntity> new_round_entity_;

    zaf::ReplaySubject<ChatCompletion> answer_event_;
    zaf::ReplaySubject<DialogSavedInfo> dialog_saved_event_;
    zaf::ReplaySubject<RoundSavedInfo> round_saved_event_;
    zaf::ReplaySubject<zaf::None> finish_event_;
};

}