#pragma once

#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/command.h"

namespace ra::mod::chat_gpt {

class ChatGPTCommand : public Command {
public:
    static constexpr wchar_t HeadingChar = L'>';

public:
    explicit ChatGPTCommand(std::shared_ptr<comm::OpenAIClient> client);

    std::wstring GetKeyword() override;

    help::HelpContent GetHelpContent() override;

    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;

    std::wstring GetPreviewText() override;

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;

    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    std::shared_ptr<comm::OpenAIClient> client_;
};

}