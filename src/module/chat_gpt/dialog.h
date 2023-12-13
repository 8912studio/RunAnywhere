#pragma once

#include <deque>
#include <zaf/rx/observable.h>
#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/chat_gpt/message.h"

namespace ra::mod::chat_gpt {

class Dialog {
public:
    explicit Dialog(std::shared_ptr<comm::OpenAIClient> client);

    zaf::Observable<comm::ChatCompletion> Chat(std::wstring content);

private:
    class Round {
    public:
        Message question;
        Message answer;
    };

private:
    std::shared_ptr<comm::OpenAIClient> client_;

    std::deque<Round> history_rounds_;
    std::size_t max_history_rounds_count_{ 5 };

    std::optional<Message> ongoing_question_;
};

}