#pragma once

#include <zaf/base/non_copyable.h>
#include "module/command.h"
#include "module/command_brief.h"
#include "utility/command_line.h"

namespace ra::module::tool {

class ToolCommandFactory : zaf::NonCopyable {
public:
    virtual std::shared_ptr<Command> CreateCommand(const utility::CommandLine& command_line) = 0;

    const CommandBrief& CommandBrief() const {
        return command_brief_;
    }

protected:
    ToolCommandFactory(const module::CommandBrief& command_brief);

private:
    module::CommandBrief command_brief_;
};

}