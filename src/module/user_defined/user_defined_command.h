#pragma once

#include <vector>
#include "module/command.h"
#include "module/user_defined/command_line_parsing.h"
#include "module/user_defined/user_defined_entry.h"

class UserDefinedCommand : public Command {
public:
    UserDefinedCommand(
        const UserDefinedEntry& entry, 
        const std::vector<std::wstring>& input_arguments);

    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    ParseResult ParseCommandLine();

private:
    UserDefinedEntry entry_;
    std::vector<std::wstring> input_arguments_;
};