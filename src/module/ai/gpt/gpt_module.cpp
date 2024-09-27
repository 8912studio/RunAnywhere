#include "module/ai/gpt/gpt_module.h"
#include "module/ai/gpt/dialog/unified_dialog_window.h"
#include "module/ai/gpt/gpt_command.h"
#include "module/ai/gpt/gpt_command_parsing.h"
#include "utility/data_directory.h"

namespace ra::mod::ai::gpt {

std::vector<CommandBrief> GPTModule::QuerySuggestedCommands(const std::wstring& command_text) {
    return std::vector<CommandBrief>();
}


std::unique_ptr<Command> GPTModule::CreateCommand(const utility::CommandLine& command_line) {

    if (!IsGPTCommand(command_line)) {
        return nullptr;
    }

    Initialize();

    return std::make_unique<GPTCommand>(context_);
}


void GPTModule::Initialize() {

    std::call_once(init_once_flag_, [this]() {

        client_ = std::make_shared<OpenAIClient>();

        auto storage_path = utility::GetDataDirectoryPath() / "GPT" / "GPT.db";
        storage_ = std::make_shared<GPTStorage>(storage_path);

        dialog_manager_ = std::make_shared<DialogManager>(client_, storage_);
        dialog_manager_->Initialize();

        auto unified_dialog_window = zaf::Create<UnifiedDialogWindow>(dialog_manager_);
        context_ = std::make_shared<GPTModuleContext>(std::move(unified_dialog_window));
    });
}

}