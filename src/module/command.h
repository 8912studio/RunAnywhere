#pragma once

#include <string>
#include "context/desktop_context.h"

class Command {
public:
    Command() = default;
    virtual ~Command() = default;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    const DesktopContext& GetDesktopContext() const {
        return desktop_context_;
    }

    void SetDesktopContext(const DesktopContext& desktop_context) {
        desktop_context_ = desktop_context;
    }

    virtual std::wstring GetPreviewText() = 0;
    virtual void Execute() = 0;

private:
    DesktopContext desktop_context_;
};