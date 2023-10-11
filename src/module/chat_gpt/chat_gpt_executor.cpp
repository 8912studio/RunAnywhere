#include "module/chat_gpt/chat_gpt_executor.h"

namespace ra::mod::chat_gpt {

ChatGPTExecutor::ChatGPTExecutor(std::shared_ptr<comm::OpenAIClient> client, std::wstring input) :
    client_(std::move(client)),
    input_(std::move(input)) {

}


void ChatGPTExecutor::Execute() {

    Message message{ std::move(input_) };
    client_->CreateChatCompletion({ std::move(message) });
}

}