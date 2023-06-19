#pragma once

#include <string>
#include <zaf/base/non_copyable.h>
#include "context/desktop_context.h"
#include "help/content/content.h"
#include "module/command_brief.h"
#include "module/command_preview_control.h"
#include "utility/command_line.h"

namespace ra::mod {

class Command : zaf::NonCopyableNonMovable {
public:
    Command() = default;
    virtual ~Command() = default;

    /**
    Gets brief info of current command.
    */
    virtual CommandBrief GetBrief() = 0;

    /**
    Gets help content of current command.
    */
    virtual help::content::Content GetHelpContent() {
        return {};
    }

    /**
    Interprets specified command line, preparing for executing.

    @param command_line
        The command line to be interpreted.

    @param desktop_context
        Desktop context information.

    @param is_reusing
        Indicates that whether current command is being reused for another command line. If the 
        command isn't reusable, return false if is_reusing is true.

    @return
        Returns true if command line is interpreted successfully; otherwise returns false.
    */
    virtual bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) = 0;

    virtual std::wstring GetPreviewText() {
        return {};
    }

    virtual std::shared_ptr<CommandPreviewControl> GetPreviewControl() {
        return nullptr;
    }

    virtual void Execute() = 0;
};

}