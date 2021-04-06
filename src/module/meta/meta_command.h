#pragma once

#include "module/command.h"

class MetaCommand : public Command {
public:
    MetaCommand(const std::wstring& command_text);

    std::wstring GetPreviewText() override;
    void Execute() override;

private:
    std::wstring command_text_;
};