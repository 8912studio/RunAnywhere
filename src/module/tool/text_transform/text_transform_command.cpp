#include "module/tool/text_transform/text_transform_command.h"
#include <zaf/creation.h>
#include "module/common/copy_executor.h"

namespace ra::mod::tool::text_transform {

TextTransformCommand::TextTransformCommand(std::unique_ptr<TextTransformer> transformer) :
    transformer_(std::move(transformer)),
    preview_control_(zaf::Create<TextPreviewControl>()) {

}


std::wstring TextTransformCommand::GetKeyword() {
    return transformer_->GetKeyword();
}


std::shared_ptr<help::markdown::element::Element> TextTransformCommand::GetHelpContent() {
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

        transformed_text_ = arguments.front().Content();
        transformer_->Transform(transformed_text_);
    }
    else {
        transformed_text_.clear();
    }

    preview_control_->SetText(transformed_text_);

    executor_ = CopyExecutor::TryCreate(transformed_text_);

    return true;
}


std::shared_ptr<CommandPreviewControl> TextTransformCommand::GetPreviewControl() {
    return preview_control_;
}


std::shared_ptr<CommandExecutor> TextTransformCommand::GetExecutor() {
    return executor_;
}

}