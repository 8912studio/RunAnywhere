#pragma once

#include <vector>
#include "context/active_path.h"
#include "module/command.h"
#include "module/user_defined/command_line_parsing.h"
#include "module/user_defined/user_defined_entry.h"

namespace ra::module::user_defined {

class UserDefinedCommand : public Command {
public:
    UserDefinedCommand(
        const UserDefinedEntryLegacy& entry,
        const std::vector<std::wstring>& input_arguments);

    help::content::Content GetHelpContent() override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    ParseResult ParseCommandLine();
    void ParseArguments(
        context::ActivePath& modified_active_path,
        std::vector<std::wstring>& plain_arguments);

private:
    UserDefinedEntryLegacy entry_;
    std::vector<std::wstring> input_arguments_;
};

}