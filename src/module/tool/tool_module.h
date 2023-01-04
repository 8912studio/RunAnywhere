#pragma once

#include "module/module.h"

namespace ra::module::tool {

class ToolCommandInfo;

class ToolModule : public Module {
public:
    ToolModule();
    ~ToolModule();

    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;
    std::shared_ptr<Command> Interpret(const utility::CommandLine& command_line) override;

private:
    std::vector<std::unique_ptr<ToolCommandInfo>> command_infos_;
};

}