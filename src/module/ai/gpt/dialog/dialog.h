#pragma once

#include <deque>
#include <zaf/base/event/event.h>
#include <zaf/base/non_copyable.h>
#include <zaf/base/none.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/round.h"

namespace ra::mod::ai::gpt {

class Dialog : public zaf::SubscriptionHost, zaf::NonCopyableNonMovable {
public:
    Dialog(std::size_t number, std::shared_ptr<OpenAIClient> client);

    std::wstring Subject() const;
    zaf::Observable<zaf::None> SubjectUpdatedEvent() const;

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
    void OnRoundRemoved(std::uint64_t round_id);

private:
    std::size_t number_{};
    std::shared_ptr<OpenAIClient> client_;

    std::uint64_t round_id_seed_{};
    //The first element is the oldest round.
    std::deque<RoundData> history_rounds_;
    std::size_t max_history_rounds_count_{ 5 };

    std::optional<Message> ongoing_question_;

    std::wstring subject_;
    zaf::Event<zaf::None> subject_updated_event_;
};

}