#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/modifier.h"
#include "module/command_preview_control.h"

class CalculatorPreviewControl : public CommandPreviewControl {
public:
	ZAF_DECLARE_REFLECTION_TYPE;

	void SetResult(
		const calculator::EvaluateResult& evaluate_result, 
		const calculator::Modifier& modifier);

protected:
	void AfterParsing() override;
	void Layout(const zaf::Rect& previous_rect) override;

private:
	void UpdateResult();
	void SetTextToLabel();
	void ShowHighlightBit();
	std::optional<std::size_t> GetHighlightBitPositionInResultLabel();
	void ResizetLabelToSuitableSize();
	void RePositionLabel();

private:
	ZAF_BIND_CONTROL(zaf::Label, resultLabel);

	calculator::EvaluateResult evaluate_result_;
	calculator::Modifier modifier_;

	std::size_t prefix_length_;
};