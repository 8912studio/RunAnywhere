#pragma once

#include <optional>
#include "module/ai/gpt/chat_gpt_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::mod::ai::gpt {

constexpr wchar_t HeadingChar = L'>';

bool IsChatGPTCommand(const utility::CommandLine& command_line);

std::optional<ChatGPTCommandParseResult> ParseChatGPTCommand(
    const utility::CommandLine& command_line);

}