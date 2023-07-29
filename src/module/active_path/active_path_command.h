#pragma once

#include "module/command.h"
#include "module/active_path/active_path_option.h"

namespace ra::mod::active_path {

class ActivePathCommand : public Command {
public:
    static CommandBrief Brief();
    static constexpr wchar_t PrefixChar = L'@';

public:
    explicit ActivePathCommand(const ActivePathOption& option);

    CommandBrief GetBrief() override;
    help::content::Content GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line, 
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::wstring GetPreviewText() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    ActivePathOption option_;
    context::DesktopContext desktop_context_;
};

}