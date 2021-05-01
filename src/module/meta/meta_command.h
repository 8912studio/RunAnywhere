#pragma once

#include "module/command.h"

namespace ra::module::meta{

class MetaCommand : public Command {
public:
    MetaCommand(const std::wstring& command_text);

    std::wstring GetPreviewText() override;
    void Execute() override;

private:
    std::wstring command_text_;
};

}