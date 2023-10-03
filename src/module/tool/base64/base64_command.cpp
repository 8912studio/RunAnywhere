#include "module/tool/base64/base64_command.h"
#include <zaf/creation.h>
#include "help/built_in_help_content_manager.h"
#include "module/common/copy_executor.h"

namespace ra::mod::tool::base64 {

std::wstring Base64Command::Keyword() {
    return L"b64";
}


std::wstring Base64Command::GetKeyword() {
    return Keyword();
}


help::HelpContent Base64Command::GetHelpContent() {
    return { 
        Keyword(), 
        help::BuiltInHelpContentManager::Instance().GetDetail(Keyword()) 
    };
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