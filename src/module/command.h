#pragma once

#include <string>

class Command {
public:
    Command() = default;
    virtual ~Command() = default;

    Command(const Command&) = delete;
    Command& operator=(const Command&) = delete;

    virtual std::wstring GetPreviewText() = 0;
    virtual void Execute() = 0;
};