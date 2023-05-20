#include "module/tool/text_transform/lower_command.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/creation.h>
#include "module/tool/text_transform/text_preview_control.h"
#include "utility/clipboard.h"

namespace ra::module::tool::text_transform {

CommandBrief LowerCommand::Brief() {
    return CommandBrief{ L"low", L"" };
}


LowerCommand::LowerCommand(const utility::CommandLine& command_line) {

    const auto& arguments = command_line.Arguments();
    if (!arguments.empty()) {
        lowered_text_ = zaf::ToLowercased(arguments.front());
    }
}


CommandBrief LowerCommand::GetBrief() {
    return Brief();
}


help::content::Content LowerCommand::GetHelpContent() {
    return {};
}


std::shared_ptr<CommandPreviewControl> LowerCommand::GetPreviewControl() {

    auto preview_control = zaf::Create<TextPreviewControl>();
    preview_control->SetText(lowered_text_);
    return preview_control;
}


void LowerCommand::Execute() {
    utility::SetStringToClipboard(lowered_text_);
}

}