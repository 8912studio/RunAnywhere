#include "module/chat_gpt/chat_gpt_command_parsing.h"
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>
#include <zaf/base/string/trim.h>

namespace ra::mod::chat_gpt {
namespace {

std::optional<std::size_t> FindHeadingCharIndex(std::wstring_view text) {

    for (auto index : zaf::Range(0, text.length())) {

        if (std::iswspace(text[index])) {
            continue;
        }

        if (text[index] == HeadingChar) {
            return index;
        }
    }

    return std::nullopt;
}

}

bool IsChatGPTCommand(const utility::CommandLine& command_line) {

    auto heading_char_index = FindHeadingCharIndex(command_line.RawText());
    return heading_char_index.has_value();
}


std::optional<ChatGPTCommandParseResult> ParseChatGPTCommand(
    const utility::CommandLine& command_line) {

    const auto& raw_text = command_line.RawText();

    auto heading_char_index = FindHeadingCharIndex(raw_text);
    if (!heading_char_index) {
        return std::nullopt;
    }

    auto arguments = command_line.Arguments();
    std::size_t object_index{};

    std::wstring content;
    for (auto index = *heading_char_index + 1; index < raw_text.length(); ++index) {

        auto ch = raw_text[index];
        if (ch != utility::ObjectReplacementChar) {

            //Insert space between object text and plain text.
            if (ch != L' ' && raw_text[index - 1] == utility::ObjectReplacementChar) {
                if (!content.empty() && content.back() != L' ') {
                    content += L' ';
                }
            }
            
            content += ch;
            continue;
        }

        //Insert space between plain text and object text.
        if (!content.empty() && content.back() != L' ') {
            content += L' ';
        }

        for (; object_index < arguments.size(); ++object_index) {

            const auto& piece = arguments[object_index];
            auto object_type = piece.Type();

            if (object_type == utility::CommandLinePieceType::TextBlock) {
                content += piece.Content();
                break;
            }
            else if (object_type == utility::CommandLinePieceType::ActivePath) {
                //Ignore active path object.
                break;
            }
        }
    }

    zaf::Trim(content);

    ChatGPTCommandParseResult result;
    result.question = std::move(content);
    return result;
}

}