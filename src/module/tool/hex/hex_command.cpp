#include "module/tool/hex/hex_command.h"
#include <zaf/creation.h>
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


help::content::Content HexCommand::GetHelpContent() {

    help::content::Content result;

    result.AddTitleLine(L"hex command");
    result.AddBodyLine(L"Display bytes of active file's content in hex and ASCII format.");

    result.AddTitleLine(L"Usage");
    result.AddBodyLine(L"hex [position] [length]");
    result.AddBodyLine(L"`position` specifies the begin position of content to display. "
        "Default is 0 if it is omitted.");
    result.AddBodyLine(L"`length` specifies the length of content to display. "
        "It is prefixed with letter `l` or `L`. Default is 128 if it is omitted. "
        "The max supported length is 4096. Use two `l` or `L` letters to specify the max length, "
        "for example `ll`.");
    result.AddBodyLine(L"Both `position` and `length` can be in decimal or hexadecimal format. "
        "Use `x` or `0x` as prefix for hexadecimal.");

    return result;
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