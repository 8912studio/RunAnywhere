#include "module/tool/json/json_command.h"
#include <zaf/creation.h>
#include "module/common/copy_executor.h"

namespace ra::mod::tool::json {

std::wstring JSONCommand::Keyword() {
    return L"json";
}


std::wstring JSONCommand::GetKeyword() {
    return Keyword();
}


help::HelpContent JSONCommand::GetHelpContent() {
    return help::HelpContent{ L"", nullptr };
}


bool JSONCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context, 
    bool is_reusing) {

    if (is_reusing) {
        return false;
    }

    parse_result_ = ParseJSONCommand(command_line);

    preview_control_ = zaf::Create<TextPreviewControl>();
    preview_control_->SetText(parse_result_.formatted_result);

    executor_ = CopyExecutor::TryCreate(parse_result_.formatted_result);
    return true;
}


std::shared_ptr<CommandPreviewControl> JSONCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> JSONCommand::GetExecutor() {
    return executor_;
}

}