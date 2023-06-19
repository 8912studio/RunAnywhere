#pragma once

#include <memory>
#include <string>
#include <zaf/base/non_copyable.h>
#include "help/content/content.h"
#include "module/command.h"
#include "module/command_brief.h"
#include "utility/command_line.h"

namespace ra::mod {

class Module : zaf::NonCopyableNonMovable {
public:
    Module() = default;
    virtual ~Module() = default;

    virtual std::vector<CommandBrief> QuerySuggestedCommands(const std::wstring& command_text) {
        return {};
    }

    virtual std::unique_ptr<Command> CreateCommand(const utility::CommandLine& command_line) = 0;
};

}