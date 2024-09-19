#pragma once

#include <optional>
#include "module/ai/gpt/gpt_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::mod::ai::gpt {

constexpr wchar_t HeadingChar = L'>';

bool IsGPTCommand(const utility::CommandLine& command_line);

std::optional<GPTCommandParseResult> ParseGPTCommand(
    const utility::CommandLine& command_line);

}