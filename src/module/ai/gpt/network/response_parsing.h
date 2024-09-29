#pragma once

#include <optional>
#include "module/ai/gpt/network/chat_completion.h"

namespace ra::mod::ai::gpt {

std::optional<ChatCompletion> ParseChatCompletion(const std::string& response) noexcept;

std::string ParseErrorMessage(const std::string& response) noexcept;

}