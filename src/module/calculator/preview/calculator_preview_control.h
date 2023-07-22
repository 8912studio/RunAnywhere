#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/modifier.h"
#include "module/command_preview_control.h"

namespace ra::mod::calculator {

class CalculatorPreviewControl : public CommandPreviewControl {
public:
	ZAF_DECLARE_TYPE;

	void SetResult(
		const calculator::EvaluateResult& evaluate_result,
		const calculator::Modifier& modifier);

protected:
	void OnStyleChanged() override;
	void Layout(const zaf::Rect& previous_rect) override;

private:
	void ResizetLabelToSuitableSize();
	void RePositionLabel();

	void UpdateResult();
	void SetTextToLabel();
	void ShowHighlightBit();
	std::optional<std::size_t> GetHighlightBitPositionInResultLabel();

private:
	ZAF_BIND_CONTROL(zaf::Label, resultLabel);

	calculator::EvaluateResult evaluate_result_;
	calculator::Modifier modifier_;

	std::size_t prefix_length_;
};

}