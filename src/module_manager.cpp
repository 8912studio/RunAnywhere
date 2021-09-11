#include "module_manager.h"
#include <zaf/base/container/utility/append.h>
#include <zaf/base/string/trim.h>
#include "module/active_path/active_path_module.h"
#include "module/calculator/calculator_module.h"
#include "module/crypto/crypto_module.h"
#include "module/date/date_module.h"
#include "module/meta/meta_module.h"
#include "module/rgb/rgb_module.h"
#include "module/user_defined/user_defined_module.h"

namespace ra {

void ModuleManager::Initialize() {

    user_defined_module_ = std::make_shared<module::user_defined::UserDefinedModule>();
    user_defined_module_->Reload();

    modules_.push_back(std::make_shared<module::meta::MetaModule>());
    modules_.push_back(std::make_shared<module::active_path::ActivePathModule>());
    modules_.push_back(std::make_shared<module::date::DateModule>());
    modules_.push_back(std::make_shared<module::crypto::CryptoModule>());
    modules_.push_back(std::make_shared<module::rgb::RGBModule>());
    modules_.push_back(user_defined_module_);
    modules_.push_back(std::make_shared<module::calculator::CalculatorModule>());
}


void ModuleManager::ReloadUserDefinedCommands() {

    if (user_defined_module_) {
        user_defined_module_->Reload();
    }
}


std::vector<module::CommandBrief> ModuleManager::QuerySuggestedCommands(
    const std::wstring& command_text) {

    auto trimmed_command_text = zaf::ToTrimmed(command_text);

    std::vector<module::CommandBrief> result;

    for (const auto& each_module : modules_) {

        auto suggested_commands = each_module->QuerySuggestedCommands(trimmed_command_text);
        zaf::Append(result, suggested_commands);
    }

    return result;
}


std::shared_ptr<module::Command> ModuleManager::InterpretCommand(const std::wstring& command_text) {

    auto trimmed_command_text = zaf::ToTrimmed(command_text);
    if (trimmed_command_text.empty()) {
        return nullptr;
    }

    utility::CommandLine command_line(trimmed_command_text);

    for (const auto& each_module : modules_) {

        auto command = each_module->Interpret(command_line);
        if (command) {
            return command;
        }
    }

    return nullptr;
}

}