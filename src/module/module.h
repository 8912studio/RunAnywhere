#pragma once

#include <memory>
#include <string>
#include "help/content/content.h"
#include "module/command.h"
#include "module/command_brief.h"
#include "utility/command_line.h"

namespace ra::module {

class Module {
public:
    Module() = default;
    virtual ~Module() = default;

    Module(const Module&) = delete;
    Module& operator=(const Module&) = delete;

    virtual std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) {
        return {};
    }

    virtual std::shared_ptr<Command> Interpret(const utility::CommandLine& command_line) = 0;
};

}