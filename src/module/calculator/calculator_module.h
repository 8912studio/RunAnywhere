#pragma once

#include "module/module.h"

namespace ra::module::calculator {

class CalculatorModule : public Module {
public:
    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;
    std::unique_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;
};

}