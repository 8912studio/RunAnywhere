#pragma once

#include <deque>
#include <zaf/base/non_copyable.h>
#include <zaf/rx/observable.h>
#include <zaf/rx/subscription_host.h>
#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/chat_gpt/dialog/round.h"

namespace ra::mod::chat_gpt {

class Dialog : public zaf::SubscriptionHost, zaf::NonCopyableNonMovable {
public:
    explicit Dialog(std::shared_ptr<comm::OpenAIClient> client);

    std::shared_ptr<Round> CreateRound(std::wstring question);

private:
    class RoundData {
    public:
        std::uint64_t id{};
        Message question;
        Message answer;
    };

private:
    zaf::Observable<comm::ChatCompletion> Chat(std::uint64_t round_id, std::wstring question);
    void OnRoundRemoved(std::uint64_t round_id);

private:
    std::shared_ptr<comm::OpenAIClient> client_;

    std::uint64_t round_id_seed_{};
    std::deque<RoundData> history_rounds_;
    std::size_t max_history_rounds_count_{ 5 };

    std::optional<Message> ongoing_question_;
};

}