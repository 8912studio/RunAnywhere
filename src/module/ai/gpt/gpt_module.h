#pragma once

#include <mutex>
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/dialog/dialog_manager.h"
#include "module/module.h"

namespace ra::mod::ai::gpt {

class GPTModule : public Module {
public:
    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;

    std::unique_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;

private:
    void Initialize();

private:
    std::once_flag init_once_flag_;
    std::shared_ptr<OpenAIClient> client_;
    std::shared_ptr<DialogManager> dialog_manager_;
};

}