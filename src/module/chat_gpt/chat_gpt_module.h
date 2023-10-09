#pragma once

#include <mutex>
#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/module.h"

namespace ra::mod::chat_gpt {

class ChatGPTModule : public Module {
public:
    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;

    std::unique_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;

private:
    const std::shared_ptr<comm::OpenAIClient>& GetClient();

private:
    std::shared_ptr<comm::OpenAIClient> client_;
    std::once_flag client_once_flag_;
};

}