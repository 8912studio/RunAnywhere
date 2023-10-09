#pragma once

#include <memory>
#include <string>
#include "module/chat_gpt/chat_gpt_module.h"
#include "module/extension/extension_module_manager.h"
#include "module/user_defined/user_defined_module.h"
#include "utility/command_line.h"

namespace ra {

class ModuleManager {
public:
    void Initialize();

    mod::user_defined::UserDefinedModule& GetUserDefinedModule();

    std::vector<mod::CommandBrief> QuerySuggestedCommands(const std::wstring& command_text);

    std::unique_ptr<mod::Command> CreateCommand(const utility::CommandLine& command_line);
    
private:
    std::shared_ptr<mod::user_defined::UserDefinedModule> user_defined_module_;
    std::shared_ptr<mod::chat_gpt::ChatGPTModule> chat_gpt_module_;
    std::unique_ptr<mod::extension::ExtensionModuleManager> extension_module_manager_;
    std::vector<std::shared_ptr<mod::Module>> modules_;
};

}