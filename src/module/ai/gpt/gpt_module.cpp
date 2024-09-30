#include "module/ai/gpt/gpt_module.h"
#include "module/ai/gpt/dialog/dialog_service.h"
#include "module/ai/gpt/dialog/unified_dialog_window.h"
#include "module/ai/gpt/gpt_command.h"
#include "module/ai/gpt/gpt_command_parsing.h"
#include "module/ai/gpt/network/open_ai_client.h"
#include "module/ai/gpt/storage/gpt_storage.h"
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

        auto client = std::make_shared<OpenAIClient>();

        auto storage_path = utility::GetDataDirectoryPath() / "GPT" / "GPT.db";
        auto storage = std::make_shared<GPTStorage>(storage_path);

        auto dialog_service = std::make_shared<DialogService>(
            std::move(client), 
            std::move(storage));

        context_ = std::make_shared<GPTModuleContext>(std::move(dialog_service));
    });
}

}