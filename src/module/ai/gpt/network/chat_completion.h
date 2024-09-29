#pragma once

#include <optional>
#include "module/ai/gpt/dialog/message.h"

namespace ra::mod::ai::gpt {

class TokenUsage {
public:
    std::size_t prompt_tokens{};
    std::size_t completion_tokens{};
    std::size_t total_tokens{};
};

class ChatCompletion {
public:
    ChatCompletion(Message message, const std::optional<TokenUsage>& token_usage) :
        message_(std::move(message)),
        token_usage_(token_usage) {
    
    }

    const Message& Message() const {
        return message_;
    }

    const std::optional<gpt::TokenUsage>& TokenUsage() const {
        return token_usage_;
    }

private:
    gpt::Message message_;
    std::optional<gpt::TokenUsage> token_usage_;
};

}