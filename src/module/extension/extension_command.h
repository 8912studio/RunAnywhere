#pragma once

#include "module/command.h"
#include "module/extension/extension_interface.h"
#include "module/extension/sdk/run_anywhere_extension_sdk.h"

namespace ra::mod::extension {

class ExtensionCommand : public Command {
public:
    ExtensionCommand(RA_Command command, const ExtensionInterface& extension_interface);
    ~ExtensionCommand();

    std::wstring GetKeyword() override;
    help::HelpContent GetHelpContent() override;
    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::wstring GetPreviewText() override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    RA_Command command_{};
    ExtensionInterface interface_;
};

}
