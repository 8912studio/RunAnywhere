#include "module/calculator/calculator_command.h"
#include <zaf/creation.h>
#include "module/calculator/preview/calculator_preview_control.h"
#include "module/calculator/result_text_builder.h"
#include "utility/clipboard.h"

namespace ra::module::calculator {

CalculatorCommand::CalculatorCommand(
    const calculator::EvaluateResult& evaluate_result,
    const calculator::Modifier& modifier)
    :
    evaluate_result_(evaluate_result),
    modifier_(modifier) {

}


std::shared_ptr<CommandPreviewControl> CalculatorCommand::GetPreviewControl() {

    auto control = zaf::Create<CalculatorPreviewControl>();
    control->SetResult(evaluate_result_, modifier_);
    return control;
}


void CalculatorCommand::Execute() {

    ResultTextBuilder text_builder(evaluate_result_, modifier_);
    auto result_text = text_builder.Build();

    utility::SetStringToClipboard(result_text.GetCompleteText());
}

}