#include "module/tool/text_transform/text_transform_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::mod::tool::text_transform {

TextTransformCommand::TextTransformCommand(std::unique_ptr<TextTransformer> transformer) :
    transformer_(std::move(transformer)),
    preview_control_(zaf::Create<TextPreviewControl>()) {

}


CommandBrief TextTransformCommand::GetBrief() {
    return transformer_->GetBrief();
}


help::content::Content TextTransformCommand::GetHelpContent() {
    return transformer_->GetHelpContent();
}


bool TextTransformCommand::Interpret(
    const utility::CommandLine & command_line,
    const context::DesktopContext & desktop_context,
    bool is_reusing) {

    if (is_reusing) {
        return false;
    }

    const auto& arguments = command_line.Arguments();
    if (!arguments.empty()) {

        transformed_text_ = arguments.front();
        transformer_->Transform(transformed_text_);
    }
    else {
        transformed_text_.clear();
    }

    preview_control_->SetText(transformed_text_);
    return true;
}


std::shared_ptr<CommandPreviewControl> TextTransformCommand::GetPreviewControl() {
    return preview_control_;
}


void TextTransformCommand::Execute() {
    utility::SetStringToClipboard(transformed_text_);
}

}