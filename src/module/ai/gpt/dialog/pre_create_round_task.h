#pragma once

#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/dialog/dialog.h"
#include "module/ai/gpt/dialog/dialog_service_event_infos.h"
#include "module/ai/gpt/dialog/round.h"
#include "module/ai/gpt/network/open_ai_client.h"

namespace ra::mod::ai::gpt {

struct CreateRoundParameters {
    std::shared_ptr<Dialog> dialog;
    RoundTransientID round_transient_id;
    std::wstring question;
    RoundList history_rounds;
    bool has_ongoing_pre_task{};
};

class PreCreateRoundTask : zaf::SubscriptionHost {
public:
    explicit PreCreateRoundTask(std::shared_ptr<OpenAIClient> client);

    void Run(const CreateRoundParameters& parameters);

    const std::shared_ptr<Dialog>& GetDialog() const {
        return dialog_;
    }

    const std::shared_ptr<Round>& GetRound() const {
        return round_;
    }

    zaf::Observable<DialogUpdatedInfo> DialogUpdatedEvent() const {
        return dialog_updated_event_.AsObservable();
    }

    zaf::Observable<RoundCreatedInfo> RoundCreatedEvent() const {
        return round_created_event_.AsObservable();
    }

    zaf::Observable<ChatResult> TaskFinishedEvent() const {
        return task_finished_event_.AsObservable();
    };

private:
    void UpdateDialog(
        const std::shared_ptr<Dialog>& dialog, 
        std::time_t update_time,
        const std::wstring& question);

    void CreateRound(
        RoundTransientID round_transient_id, 
        std::time_t update_time, 
        const std::wstring& question);

    void CreateChat(
        const std::wstring& question, 
        const RoundList& history_rounds,
        bool has_ongoing_pre_task);

private:
    std::shared_ptr<OpenAIClient> client_;

    std::shared_ptr<Dialog> dialog_;
    std::shared_ptr<Round> round_;

    zaf::ReplaySubject<DialogUpdatedInfo> dialog_updated_event_;
    zaf::ReplaySubject<RoundCreatedInfo> round_created_event_;
    zaf::ReplaySubject<ChatCompletion> round_finished_event_;
    zaf::ReplaySubject<ChatResult> task_finished_event_;
};

}