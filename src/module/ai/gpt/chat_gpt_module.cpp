#include "module/ai/gpt/chat_gpt_module.h"
#include "module/ai/gpt/chat_gpt_command.h"
#include "module/ai/gpt/chat_gpt_command_parsing.h"

namespace ra::mod::chat_gpt {

std::vector<CommandBrief> ChatGPTModule::QuerySuggestedCommands(const std::wstring& command_text) {
    return std::vector<CommandBrief>();
}


std::unique_ptr<Command> ChatGPTModule::CreateCommand(const utility::CommandLine& command_line) {

    if (!IsChatGPTCommand(command_line)) {
        return nullptr;
    }

    Initialize();

    return std::make_unique<ChatGPTCommand>(dialog_manager_);
}


void ChatGPTModule::Initialize() {

    std::call_once(init_once_flag_, [this]() {
        client_ = std::make_shared<comm::OpenAIClient>();
        dialog_manager_ = std::make_shared<DialogManager>(client_);
    });
}

}