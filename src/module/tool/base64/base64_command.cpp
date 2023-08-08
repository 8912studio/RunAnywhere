#include "module/tool/base64/base64_command.h"
#include <zaf/creation.h>
#include "module/common/copy_executor.h"

namespace ra::mod::tool::base64 {

CommandBrief Base64Command::Brief() {
    return CommandBrief{ L"b64", L"Base64 encoding and decoding" };
}


CommandBrief Base64Command::GetBrief() {
    return Brief();
}


help::content::Content Base64Command::GetHelpContent() {

    help::content::Content result;
    result.AddTitleLine(L"Base64 command");
    result.AddBodyLine(L"Base64 encode or decode specified text.");

    result.AddTitleLine(L"Usage");
    result.AddBodyLine(L"b64 <text> [switches]");
    result.AddBodyLine(
        L"If there are space characters in `text`, enclose it in double quotation marks( \" ). "
        L"Or use text block(press ALT+T to insert) to contain arbitrary text."
    );
    result.AddBodyLine(
        L"The command detects if `text` is a Base64-encoded string, if so, decoding mode is used; "
        L"otherwise encoding mode is used."
    );
    result.AddTitleLine(L"Switches");
    result.AddTwoPartsLine(L"/e", L"Use encoding mode");
    result.AddTwoPartsLine(L"/d", L"Use decoding mode");
    result.AddTwoPartsLine(L"/u8", L"Encode `text` in UTF-8 encoding");
    result.AddTwoPartsLine(L"/u16", L"Encode `text` in UTF-16 encoding");
    result.AddTwoPartsLine(L"/h", L"Display decoded content in hex format");

    return result;
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