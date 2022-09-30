#pragma once

#include "module/module.h"
#include "module/tool/tool_command_factory.h"

namespace ra::module::tool {

class ToolModule : public Module {
public:
    ToolModule();

    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;

    std::shared_ptr<Command> Interpret(const utility::CommandLine& command_line) override;

private:
    std::vector<std::unique_ptr<ToolCommandFactory>> command_factories_;
};

}