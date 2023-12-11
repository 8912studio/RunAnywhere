#include "module/tool/json/json_command.h"
#include <zaf/creation.h>
#include "help/built_in_help_content_manager.h"
#include "module/common/copy_executor.h"
#include "module/tool/json/json_command_parsing.h"

namespace ra::mod::tool::json {

std::wstring JSONCommand::Keyword() {
    return L"json";
}


std::wstring JSONCommand::GetKeyword() {
    return Keyword();
}


help::HelpContent JSONCommand::GetHelpContent() {
    return {
        Keyword(),
        help::BuiltInHelpContentManager::Instance().GetDetail(Keyword())
    };
}


bool JSONCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context, 
    bool is_reusing) {

    if (is_reusing) {
        return false;
    }

    auto parse_result = ParseJSONCommand(command_line);

    preview_control_ = zaf::Create<JSONCommandPreviewControl>();
    preview_control_->ShowResult(parse_result);

    if (auto styled_text = parse_result.StyledText()) {
        executor_ = CopyExecutor::TryCreate(styled_text->Text());
    }
    
    return true;
}


std::shared_ptr<CommandPreviewControl> JSONCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> JSONCommand::GetExecutor() {
    return executor_;
}

}