#pragma once

#include "module/chat_gpt/comm/open_ai_client.h"
#include "module/command_executor.h"

namespace ra::mod::chat_gpt {

class ChatGPTExecutor : public CommandExecutor {
public:
    explicit ChatGPTExecutor(std::shared_ptr<comm::OpenAIClient>, std::wstring input);

    void Execute() override;

private:
    std::shared_ptr<comm::OpenAIClient> client_;
    std::wstring input_;
};

}