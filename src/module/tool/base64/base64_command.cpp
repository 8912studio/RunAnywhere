#include "module/tool/base64/base64_command.h"
#include <zaf/creation.h>
#include "help/built_in_help_content.h"
#include "module/common/copy_executor.h"

namespace ra::mod::tool::base64 {

CommandBrief Base64Command::Brief() {
    return CommandBrief{ L"b64", L"Base64 encoding and decoding" };
}


CommandBrief Base64Command::GetBrief() {
    return Brief();
}


std::shared_ptr<help::markdown::element::Element> Base64Command::GetHelpContent() {
    return help::LoadBuiltInHelpContent(Brief().Command());
}


bool Base64Command::Interpret(
    const utility::CommandLine& command_line,
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    if (is_reusing) {
        return false;
    }

    auto parse_result = Parse(command_line);
    preview_control_ = zaf::Create<Base64PreviewControl>();
    preview_control_->ShowParseResult(parse_result);

    auto text = preview_control_->GetResultText();
    executor_ = CopyExecutor::TryCreate(std::move(text));

    return true;
}


std::shared_ptr<CommandPreviewControl> Base64Command::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> Base64Command::GetExecutor() {
    return executor_;
}

}