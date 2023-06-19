#pragma once

#include "module/module.h"

namespace ra::mod::meta {

class MetaModule : public Module {
public:
    std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) override;

    std::unique_ptr<Command> CreateCommand(const utility::CommandLine& command_line) override;
};

}