#pragma once

#include <string>
#include "context/desktop_context.h"
#include "help/content/content.h"
#include "module/command_preview_control.h"

namespace ra::module {

class Command {
public:
    Command() = default;
    virtual ~Command() = default;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    const context::DesktopContext& GetDesktopContext() const {
        return desktop_context_;
    }

    void SetDesktopContext(const context::DesktopContext& desktop_context) {
        desktop_context_ = desktop_context;
    }

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