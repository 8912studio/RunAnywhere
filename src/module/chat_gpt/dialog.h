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
    std::shared_ptr<comm::OpenAIClient> client_;

    std::deque<Message> history_messages_;
    std::size_t max_history_message_count_{};

    std::optional<Message> ongoing_message_;
};

}