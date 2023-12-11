#pragma once

#include <mutex>
#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/chat_gpt/conversation.h"
#include "module/module.h"

namespace ra::mod::chat_gpt {

class ChatGPTModule : public Module {
public:
    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;

    std::unique_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;

private:
    const std::shared_ptr<comm::OpenAIClient>& GetClient();
    const std::shared_ptr<Conversation> GetConversation();
    void Initialize();

private:
    std::once_flag init_once_flag_;
    std::shared_ptr<Conversation> conversation_;
    std::shared_ptr<comm::OpenAIClient> client_;
};

}