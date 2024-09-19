#include "module/ai/gpt/gpt_module.h"
#include "module/ai/gpt/gpt_command.h"
#include "module/ai/gpt/gpt_command_parsing.h"

namespace ra::mod::ai::gpt {

std::vector<CommandBrief> GPTModule::QuerySuggestedCommands(const std::wstring& command_text) {
    return std::vector<CommandBrief>();
}


std::unique_ptr<Command> GPTModule::CreateCommand(const utility::CommandLine& command_line) {

    if (!IsGPTCommand(command_line)) {
        return nullptr;
    }

    Initialize();

    return std::make_unique<GPTCommand>(dialog_manager_);
}


void GPTModule::Initialize() {

    std::call_once(init_once_flag_, [this]() {
        client_ = std::make_shared<OpenAIClient>();
        dialog_manager_ = std::make_shared<DialogManager>(client_);
    });
}

}