#include "module/extension/extension_command.h"
#include <zaf/creation.h>
#include "utility/markdown/element/factory.h"
#include "module/common/copy_executor.h"
#include "module/extension/command_line_bridge.h"
#include "module/extension/extension_command_preview_control.h"

namespace ra::mod::extension {

ExtensionCommand::ExtensionCommand(
    RA_Command command, 
    const ExtensionInterface& extension_interface)
    : 
    command_(command),
    interface_(extension_interface) {

}


ExtensionCommand::~ExtensionCommand() {

    interface_.destroy_function(command_);
}


std::wstring ExtensionCommand::GetKeyword() {
    return {};
}


help::HelpContent ExtensionCommand::GetHelpContent() {
    return { L"", utility::markdown::element::MakeRoot({}) };
}


bool ExtensionCommand::Interpret(
    const utility::CommandLine& command_line,
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    //Not allow to resuse.
    if (is_reusing) {
        return false;
    }

    CommandLineBridge command_line_bridge{ command_line };
    return interface_.interpret_function(command_, command_line_bridge.PlainStruct());
}


std::wstring ExtensionCommand::GetPreviewText() {
    return interface_.get_preview_text_function(command_);
}


std::shared_ptr<CommandPreviewControl> ExtensionCommand::GetPreviewControl() {

    auto control = zaf::Create<ExtensionCommandPreviewControl>();
    control->SetText(GetPreviewText());
    return control;
}


std::shared_ptr<CommandExecutor> ExtensionCommand::GetExecutor() {
    return CopyExecutor::TryCreate(GetPreviewText());
}

}