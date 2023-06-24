#include "module/tool/base64/base64_command.h"
#include <zaf/creation.h>

namespace ra::mod::tool::base64 {

CommandBrief Base64Command::Brief() {
    return CommandBrief{ L"base", {} };
}


Base64Command::Base64Command(const utility::CommandLine&) {
    //Do nothing in constructor. We parse command line in Interpret() method.
}


CommandBrief Base64Command::GetBrief() {
    return Brief();
}


help::content::Content Base64Command::GetHelpContent() {
    return {};
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
    return true;
}


std::shared_ptr<CommandPreviewControl> Base64Command::GetPreviewControl() {
    return preview_control_;
}


void Base64Command::Execute() {

}

}