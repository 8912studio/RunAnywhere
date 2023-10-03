#pragma once

#include <optional>
#include "module/command.h"
#include "module/tool/base64/base64_command_parsing.h"
#include "module/tool/base64/base64_preview_control.h"

namespace ra::mod::tool::base64 {

class Base64Command : public Command {
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
    std::shared_ptr<Base64PreviewControl> preview_control_;
    std::shared_ptr<CommandExecutor> executor_;
};

}