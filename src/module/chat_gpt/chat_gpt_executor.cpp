#include "module/chat_gpt/chat_gpt_executor.h"

namespace ra::mod::chat_gpt {

ChatGPTExecutor::ChatGPTExecutor(std::shared_ptr<comm::OpenAIClient> client) : 
    client_(std::move(client)) {

}


void ChatGPTExecutor::Execute() {

    client_->CreateChatCompletion();
}

}