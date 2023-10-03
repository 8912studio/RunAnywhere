#pragma once

#include "module/command.h"
#include "module/command_brief.h"
#include "module/tool/error/error_command_parse_result.h"
#include "module/tool/error/error_preview_control.h"
#include "utility/command_line.h"

namespace ra::mod::tool::error {

class ErrorCommand : public Command {
public:
    static std::wstring Keyword();
    static std::optional<ErrorCommandParseResult> Parse(const utility::CommandLine& command_line);

public:
    std::wstring GetKeyword() override;
    help::HelpContent GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line, 
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    std::optional<ErrorCommandParseResult> parse_result_;
    std::shared_ptr<ErrorPreviewControl> preview_control_;
};

}