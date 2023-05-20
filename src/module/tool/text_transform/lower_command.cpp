#include "module/tool/text_transform/lower_command.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::module::tool::text_transform {

CommandBrief LowerCommand::Brief() {
    return CommandBrief{ L"low", L"" };
}


LowerCommand::LowerCommand(const utility::CommandLine& command_line) : 
    preview_control_(zaf::Create<TextPreviewControl>()) {

}


CommandBrief LowerCommand::GetBrief() {
    return Brief();
}


help::content::Content LowerCommand::GetHelpContent() {
    return {};
}


bool LowerCommand::Interpret(
    const utility::CommandLine& command_line,
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    const auto& arguments = command_line.Arguments();
    if (!arguments.empty()) {
        lowered_text_ = zaf::ToLowercased(arguments.front());
    }
    else {
        lowered_text_.clear();
    }

    preview_control_->SetText(lowered_text_);
    return true;
}


std::shared_ptr<CommandPreviewControl> LowerCommand::GetPreviewControl() {
    return preview_control_;
}


void LowerCommand::Execute() {
    utility::SetStringToClipboard(lowered_text_);
}

}