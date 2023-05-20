#pragma once

#include "module/command.h"
#include "module/extension/extension_interface.h"
#include "module/extension/sdk/run_anywhere_extension_sdk.h"

namespace ra::module::extension {

class ExtensionCommand : public Command {
public:
    ExtensionCommand(RunAnywhere_Command command, const ExtensionInterface& extension_interface);
    ~ExtensionCommand();

    CommandBrief GetBrief() override;
    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context,
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    void Execute() override;

private:
    RunAnywhere_Command command_{};
    ExtensionInterface interface_;
};

}
