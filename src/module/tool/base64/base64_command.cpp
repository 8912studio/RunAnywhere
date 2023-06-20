#include "module/tool/base64/base64_command.h"
#include <zaf/base/base64.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/creation.h>

namespace ra::mod::tool::base64 {

CommandBrief Base64Command::Brief() {
    return CommandBrief{ L"b64", {} };
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

    preview_control_ = zaf::Create<TextPreviewControl>();

    bool use_encode{ true };
    if (!parse_result.operation) {

        try {
            auto decoded = zaf::Base64Decode(zaf::ToUTF8String(parse_result.input_text));
            use_encode = false;
        }
        catch (const zaf::Error&) {

        }
    }

    if (use_encode) {

        auto encoded_text = zaf::Base64Encode(
            parse_result.input_text.data(),
            parse_result.input_text.length());

        preview_control_->SetText(zaf::FromUTF8String(encoded_text));
    }
    return true;
}


std::shared_ptr<CommandPreviewControl> Base64Command::GetPreviewControl() {
    return preview_control_;
}


void Base64Command::Execute() {

}

}