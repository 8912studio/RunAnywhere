#pragma once

#include <memory>
#include <string>
#include "help/content/content.h"
#include "module/extension/extension_module_manager.h"
#include "module/user_defined/user_defined_module.h"

namespace ra {

class ModuleManager {
public:
    void Initialize();

    void ReloadUserDefinedCommands();

    std::vector<module::CommandBrief> QuerySuggestedCommands(const std::wstring& command_text);

    std::shared_ptr<module::Command> InterpretCommand(const std::wstring& command_text);
    
private:
    std::shared_ptr<module::user_defined::UserDefinedModule> user_defined_module_;
    std::unique_ptr<module::extension::ExtensionModuleManager> extension_module_manager_;
    std::vector<std::shared_ptr<module::Module>> modules_;
};

}