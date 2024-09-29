#pragma once

#include "module/ai/gpt/network/chat_completion.h"

namespace ra::mod::ai::gpt {

class ChatResult {
public:
    ChatResult(gpt::ChatCompletion chat_completion, std::string response) :
        chat_completion_(std::move(chat_completion)),
        response_(std::move(response)) {

    }

    const gpt::ChatCompletion& ChatCompletion() const {
        return chat_completion_;
    }

    const std::string& Response() const {
        return response_;
    }

private:
    gpt::ChatCompletion chat_completion_;
    std::string response_;
};

}