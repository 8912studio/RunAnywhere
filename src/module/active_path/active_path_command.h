#pragma once

#include "module/command.h"
#include "module/active_path/active_path_option.h"

namespace ra::mod::active_path {

class ActivePathCommand : public Command {
public:
    static constexpr wchar_t PrefixChar = L'@';
    static constexpr const wchar_t* HelpContentIdentity = L"at";

public:
    explicit ActivePathCommand(const ActivePathOption& option);

    std::wstring GetKeyword() override;
    help::HelpContent GetHelpContent() override;
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