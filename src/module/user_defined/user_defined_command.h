#pragma once

#include <vector>
#include "context/active_path.h"
#include "module/command.h"
#include "module/user_defined/entry.h"
#include "module/user_defined/execute_info.h"

namespace ra::mod::user_defined {

class UserDefinedCommand : public Command {
public:
    UserDefinedCommand(
        const std::shared_ptr<Entry>& entry,
        const std::vector<std::wstring>& input_arguments);

    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line, 
        const context::DesktopContext& desktop_context, 
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    ExecuteInfo ParseCommand() const;
    void ParseArguments(
        context::ActivePath& modified_active_path,
        std::vector<std::wstring>& plain_arguments) const;

private:
    std::shared_ptr<Entry> entry_;
    std::vector<std::wstring> input_arguments_;
    context::DesktopContext desktop_context_;
};

}