#pragma once

#include <deque>
#include <zaf/base/event/event.h>
#include <zaf/base/non_copyable.h>
#include <zaf/base/none.h>
#include <zaf/object/object.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/round.h"
#include "module/ai/gpt/storage/dialog_entity.h"
#include "module/ai/gpt/storage/gpt_storage.h"

namespace ra::mod::ai::gpt {

class Dialog : public zaf::Object, zaf::SubscriptionHost, zaf::NonCopyableNonMovable {
public:
    Dialog(
        std::size_t transient_id, 
        DialogEntity entity, 
        std::shared_ptr<OpenAIClient> client,
        std::shared_ptr<GPTStorage> storage);

    std::size_t TransientID() const {
        return transient_id_;
    }

    const DialogEntity& Entity() const {
        return entity_;
    }

    std::wstring Subject() const;
    zaf::Observable<zaf::None> SubjectUpdatedEvent() const;

    zaf::Observable<std::shared_ptr<Dialog>> TimeUpdatedEvent() const;

    std::shared_ptr<Round> CreateRound(std::wstring question);

private:
    class RoundData {
    public:
        std::uint64_t id{};
        Message question;
        Message answer;
    };

private:
    static std::wstring GenerateSubject(const std::wstring& question);

    zaf::Observable<ChatCompletion> Chat(std::uint64_t round_id, std::wstring question);
    void SaveDialogEntity();

    void OnRoundRemoved(std::uint64_t round_id);

private:
    std::size_t transient_id_{};
    DialogEntity entity_;
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<GPTStorage> storage_;

    std::uint64_t round_id_seed_{};
    //The first element is the oldest round.
    std::deque<RoundData> history_rounds_;
    std::size_t max_history_rounds_count_{ 5 };

    std::optional<Message> ongoing_question_;

    zaf::Event<zaf::None> subject_updated_event_;
    zaf::Event<std::shared_ptr<Dialog>> time_updated_event_;
};

}