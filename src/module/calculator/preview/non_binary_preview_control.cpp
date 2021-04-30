#include "module/calculator/preview/non_binary_preview_control.h"
#include <zaf/base/container/utility/range.h>
#include <zaf/reflection/reflection_type_definition.h>
#include "module/calculator/result_text_builder.h"

namespace {

std::wstring InsertSpaceToDecimalText(const std::wstring& text) {

	auto result = text;
	std::reverse(result.begin(), result.end());

	std::size_t position{};
	auto decimal_point_position = result.find(L'.');
	if (decimal_point_position != std::wstring::npos) {
		position = decimal_point_position + 1;
	}

	std::size_t digit_count{ 0 };
	while (position < result.length()) {

		++digit_count;

		if (digit_count == 4) {

			result.insert(position, 1, L' ');
			digit_count = 1;
			position += 2;
		}
		else {

			++position;
		}
	}

	std::reverse(result.begin(), result.end());
	return result;
}

}

ZAF_DEFINE_REFLECTION_TYPE(NonBinaryPreviewControl)
ZAF_DEFINE_RESOURCE_URI(L"res:///module/calculator/preview/non_binary_preview_control.xaml");
ZAF_DEFINE_END


void NonBinaryPreviewControl::AfterParsing() {

	__super::AfterParsing();
}


void NonBinaryPreviewControl::Layout(const zaf::Rect& previous_rect) {

	LayoutResultLabel();
	LayoutEqualLabel();
}


void NonBinaryPreviewControl::LayoutResultLabel() {

	resultLabel->ResizeToPreferredSize();

	auto content_size = this->GetContentSize();
	const auto& result_label_size = resultLabel->GetSize();

	zaf::Point result_label_position;
	result_label_position.x = (content_size.width - result_label_size.width) / 2;
	result_label_position.y = (content_size.height - result_label_size.height) / 2;

	resultLabel->SetPosition(result_label_position);
}


void NonBinaryPreviewControl::LayoutEqualLabel() {

	equalLabel->ResizeToPreferredSize();

	const auto& result_label_rect = resultLabel->GetRect();
	const auto& equal_label_size = equalLabel->GetSize();
	constexpr float margin_to_result_label = 26;

	zaf::Point equal_label_position;
	equal_label_position.x = 
		result_label_rect.position.x - equal_label_size.width - margin_to_result_label;
	equal_label_position.y = 
		result_label_rect.position.y + result_label_rect.size.height - equal_label_size.height;

	equalLabel->SetPosition(equal_label_position);
}


void NonBinaryPreviewControl::SetResult(
	const calculator::EvaluateResult& evaluate_result,
	const calculator::Modifier& modifier) {

	evaluate_result_ = evaluate_result;
	modifier_ = modifier;

	UpdateResult();
}


void NonBinaryPreviewControl::UpdateResult() {

	ResultTextBuilder text_builder(evaluate_result_, modifier_);
	auto text = text_builder.Build();
	if (modifier_.base == 10) {
		text = InsertSpaceToDecimalText(text);
	}

	resultLabel->SetText(text);
	NeedRelayout();
}

