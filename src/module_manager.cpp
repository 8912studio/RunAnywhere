#include "module_manager.h"
#include <zaf/application.h>
#include <zaf/base/container/utility/append.h>
#include <zaf/base/string/trim.h>
#include "module/active_path/active_path_module.h"
#include "module/calculator/calculator_module.h"
#include "module/meta/meta_module.h"
#include "module/tool/tool_module.h"
#include "module/user_defined/user_defined_module.h"

namespace ra {

void ModuleManager::Initialize() {

    user_defined_module_ = std::make_shared<mod::user_defined::UserDefinedModule>();
    user_defined_module_->Reload();

    chat_gpt_module_ = std::make_shared<mod::chat_gpt::ChatGPTModule>();

    extension_module_manager_ = std::make_unique<mod::extension::ExtensionModuleManager>(
        zaf::Application::Instance().GetExeDirectoryPath() / L"Extensions");
    extension_module_manager_->Load();

    modules_.push_back(std::make_shared<mod::meta::MetaModule>());
    modules_.push_back(std::make_shared<mod::active_path::ActivePathModule>());
    modules_.push_back(std::make_shared<mod::tool::ToolModule>());
    modules_.push_back(user_defined_module_);
    zaf::Append(modules_, extension_module_manager_->GetAllModules());
    modules_.push_back(chat_gpt_module_);
    modules_.push_back(std::make_shared<mod::calculator::CalculatorModule>());
}


mod::user_defined::UserDefinedModule& ModuleManager::GetUserDefinedModule() {
    ZAF_EXPECT(user_defined_module_);
    return *user_defined_module_;
}


std::vector<mod::CommandBrief> ModuleManager::QuerySuggestedCommands(
    const std::wstring& command_text) {

    auto trimmed_command_text = zaf::ToTrimmed(command_text);

    std::vector<mod::CommandBrief> result;

    for (const auto& each_module : modules_) {

        auto suggested_commands = each_module->QuerySuggestedCommands(trimmed_command_text);
        zaf::Append(result, suggested_commands);
    }

    return result;
}


std::unique_ptr<mod::Command> ModuleManager::CreateCommand(
    const utility::CommandLine& command_line) {

    for (const auto& each_module : modules_) {

        auto command = each_module->CreateCommand(command_line);
        if (command) {
            return command;
        }
    }

    return nullptr;
}

}