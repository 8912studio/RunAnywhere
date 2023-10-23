#include "module/chat_gpt/chat_gpt_command_parsing.h"
#include <zaf/base/string/join.h>
#include <zaf/base/string/trim.h>

namespace ra::mod::chat_gpt {

bool IsChatGPTCommand(const utility::CommandLine& command_line) {

    const auto& raw_text = command_line.RawText();
    if (raw_text.empty()) {
        return false;
    }

    return raw_text.front() == HeadingChar;
}


std::optional<ChatGPTCommandParseResult> ParseChatGPTCommand(
    const utility::CommandLine& command_line) {

    if (!IsChatGPTCommand(command_line)) {
        return std::nullopt;
    }

    auto all_content = zaf::JoinAsWideString(command_line.AllPieceContents());
    all_content.erase(0, 1); //Remove the heading char.
    zaf::Trim(all_content);

    ChatGPTCommandParseResult result;
    result.question = std::move(all_content);
    return result;
}

}