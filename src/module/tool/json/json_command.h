#pragma once

#include "module/command.h"
#include "module/common/text_preview_control.h"
#include "module/tool/json/json_command_parsing.h"

namespace ra::mod::tool::json {

class JSONCommand : public Command {
public:
    static std::wstring Keyword();

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
    JSONCommandParseResult parse_result_;

    std::shared_ptr<TextPreviewControl> preview_control_;
    std::shared_ptr<CommandExecutor> executor_;
};

}