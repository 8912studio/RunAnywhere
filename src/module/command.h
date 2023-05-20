#pragma once

#include <string>
#include <zaf/base/non_copyable.h>
#include "context/desktop_context.h"
#include "help/content/content.h"
#include "module/command_brief.h"
#include "module/command_preview_control.h"

namespace ra::module {

class Command : zaf::NonCopyableNonMovable {
public:
    Command() = default;
    virtual ~Command() = default;

    const context::DesktopContext& GetDesktopContext() const {
        return desktop_context_;
    }

    void SetDesktopContext(const context::DesktopContext& desktop_context) {
        desktop_context_ = desktop_context;
    }

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

    virtual std::wstring GetPreviewText() {
        return {};
    }

    virtual std::shared_ptr<CommandPreviewControl> GetPreviewControl() {
        return nullptr;
    }

    virtual void Execute() = 0;

private:
    context::DesktopContext desktop_context_;
};

}