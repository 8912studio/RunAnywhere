#pragma once

#include <zaf/rx/subscription_host.h>
#include "module/ai/gpt/chat_gpt_executor.h"
#include "module/ai/gpt/chat_gpt_preview_control.h"
#include "module/ai/gpt/dialog/dialog_manager.h"
#include "module/command.h"

namespace ra::mod::ai::gpt {

class ChatGPTCommand : public Command, zaf::SubscriptionHost {
public:
    explicit ChatGPTCommand(std::shared_ptr<DialogManager> dialog_manager);
    
    std::wstring GetKeyword() override;

    help::HelpContent GetHelpContent() override;

    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;

    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    std::shared_ptr<ChatGPTPreviewControl> preview_control_;
    std::shared_ptr<ChatGPTExecutor> chat_gpt_executor_;
};

}