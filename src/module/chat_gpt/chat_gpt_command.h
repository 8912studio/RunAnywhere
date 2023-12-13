#pragma once

#include <zaf/rx/subscription_host.h>
#include "module/chat_gpt/chat_gpt_executor.h"
#include "module/chat_gpt/chat_gpt_preview_control.h"
#include "module/chat_gpt/dialog.h"
#include "module/command.h"

namespace ra::mod::chat_gpt {

class ChatGPTCommand : public Command, zaf::SubscriptionHost {
public:
    explicit ChatGPTCommand(std::shared_ptr<Dialog> dialog);
    
    std::wstring GetKeyword() override;

    help::HelpContent GetHelpContent() override;

    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;

    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    enum class CommandState {
        //The command is waiting for executing.
        Waiting,
        //The command is executing.
        Executing,
        //The command has failed.
        Failed,
        //The command has completed.
        Completed,
    };

private:
    void CreateExecutor();
    void OnBeginExecute();

private:
    std::shared_ptr<Dialog> dialog_;
    
    std::shared_ptr<ChatGPTPreviewControl> preview_control_;

    CommandState command_state_{ CommandState::Waiting };
    std::wstring question_;
    std::wstring answer_;
    
    std::shared_ptr<ChatGPTExecutor> chat_gpt_executor_;
};

}