#include "module/tool/hex/hex_command.h"
#include <zaf/creation.h>
#include "help/built_in_help_content_manager.h"
#include "module/active_path/active_path_modifying.h"
#include "module/common/command_error_control.h"
#include "module/common/general_input.h"
#include "module/tool/hex/hex_command_parsing.h"

namespace ra::mod::tool::hex {

CommandBrief HexCommand::Brief() {
    return CommandBrief{
        L"hex",
        L"Display bytes of file content in hex format",
    };
}


CommandBrief HexCommand::GetBrief() {
    return Brief();
}


std::shared_ptr<help::markdown::element::Element> HexCommand::GetHelpContent() {
    return help::BuiltInHelpContentManager::Instance().GetDetail(Brief().Command());
}


bool HexCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    //Not allow to reuse.
    if (is_reusing) {
        return false;
    }

    parse_result_ = ParseHexCommand(command_line);
    desktop_context_ = desktop_context;
    return true;
}


std::shared_ptr<CommandPreviewControl> HexCommand::GetPreviewControl() {

    if (!parse_result_) {
        return CommandErrorControl::InvalidArgument();
    }

    if (!preview_control_) {
        CreatePreviewControl();
    }

    return preview_control_;
}


void HexCommand::CreatePreviewControl() {

    preview_control_ = zaf::Create<HexPreviewControl>();
    preview_control_->ShowContent(
        MakeGeneralInput(desktop_context_, parse_result_->general_option),
        zaf::Range{ parse_result_->position, parse_result_->length });
}


std::shared_ptr<CommandExecutor> HexCommand::GetExecutor() {
    return nullptr;
}


}