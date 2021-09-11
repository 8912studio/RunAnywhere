#pragma once

#include "module/module.h"

namespace ra::module::active_path {

class ActivePathModule : public Module {
public:
    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;

    std::shared_ptr<Command> Interpret(const utility::CommandLine& command_line) override;
};

}