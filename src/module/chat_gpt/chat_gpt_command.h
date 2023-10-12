#pragma once

#include <zaf/rx/subscription_host.h>
#include "module/chat_gpt/chat_gpt_executor.h"
#include "module/chat_gpt/chat_gpt_preview_control.h"
#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/command.h"

namespace ra::mod::chat_gpt {

class ChatGPTCommand : public Command, zaf::SubscriptionHost {
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

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;

    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    void InitializeExecutor();

private:
    std::shared_ptr<comm::OpenAIClient> client_;
    std::shared_ptr<ChatGPTPreviewControl> preview_control_;
    std::shared_ptr<ChatGPTExecutor> executor_;
};

}