#include "module/calculator/preview/non_binary_preview_control.h"
#include <zaf/base/container/utility/range.h>
#include <zaf/graphic/font/font.h>
#include <zaf/reflection/reflection_type_definition.h>
#include "module/calculator/preview/numeric_text_formatting.h"
#include "module/calculator/result_text_builder.h"

namespace {

constexpr float DefaultFontSize = 32;

}

ZAF_DEFINE_REFLECTION_TYPE(NonBinaryPreviewControl)
ZAF_DEFINE_RESOURCE_URI(L"res:///module/calculator/preview/non_binary_preview_control.xaml");
ZAF_DEFINE_END


void NonBinaryPreviewControl::AfterParsing() {

	__super::AfterParsing();
}


void NonBinaryPreviewControl::Layout(const zaf::Rect& previous_rect) {

	ResizetLabelsToSuitableSize();
	RePositionLabels();
}


void NonBinaryPreviewControl::ResizetLabelsToSuitableSize() {

	auto content_size = GetContentSize();
	if (content_size.width == 0) {
		return;
	}

	if (resultLabel->GetText().length() == 0) {
		return;
	}

	for (float font_size = DefaultFontSize; font_size > 0; --font_size) {

		resultLabel->SetFontSize(font_size);
		prefixLabel->SetFontSize(font_size);

		auto result_label_size = resultLabel->GetPreferredSize();
		auto result_label_margin = resultLabel->GetMargin();

		auto prefix_label_size = prefixLabel->GetPreferredSize();
		auto prefix_label_margin = prefixLabel->GetMargin();

		auto total_width =
			result_label_size.width +
			result_label_margin.left +
			result_label_margin.right +
			prefix_label_size.width +
			prefix_label_margin.left +
			prefix_label_margin.right;

		if (total_width <= content_size.width) {

			resultLabel->ResizeToPreferredSize();
			prefixLabel->ResizeToPreferredSize();
			break;
		}
	}
}


void NonBinaryPreviewControl::RePositionLabels() {

	auto content_size = this->GetContentSize();
	const auto& result_label_size = resultLabel->GetSize();

	//First, center result label.
	zaf::Point result_label_position;
	result_label_position.x = (content_size.width - result_label_size.width) / 2;
	result_label_position.y = (content_size.height - result_label_size.height) / 2;

	//Second, move prefix label in front of result label.
	const auto& prefix_label_size = prefixLabel->GetSize();
	zaf::Point prefix_label_position;
	prefix_label_position.x =
		result_label_position.x - prefix_label_size.width;
	prefix_label_position.y =
		result_label_position.y + result_label_size.height - prefix_label_size.height;

	//Third, check if prefix label is exceeds content rect, and revise labels' positions.
	if (prefix_label_position.x < 0) {

		float revise_offset = 0 - prefix_label_position.x;
		prefix_label_position.x += revise_offset;
		result_label_position.x += revise_offset;
	}

	resultLabel->SetPosition(result_label_position);
	prefixLabel->SetPosition(prefix_label_position);
}


void NonBinaryPreviewControl::SetResult(
	const calculator::EvaluateResult& evaluate_result,
	const calculator::Modifier& modifier) {

	evaluate_result_ = evaluate_result;
	modifier_ = modifier;

	UpdateResult();
}


void NonBinaryPreviewControl::UpdateResult() {

	SetTextToLabels();
	ShowHighlightBit();
	NeedRelayout();
}


void NonBinaryPreviewControl::SetTextToLabels() {

	ResultTextBuilder text_builder(evaluate_result_, modifier_);
	auto result_text = text_builder.Build();

	InsertSpaceToNumericText(result_text.content, modifier_.base);
	resultLabel->SetText(result_text.content);

	if (!result_text.prefix.empty()) {
		prefixLabel->SetText(result_text.prefix + L' ');
	}
	else {
		prefixLabel->SetText({});
	}
}


void NonBinaryPreviewControl::ShowHighlightBit() {

	auto highlight_position = GetHighlightBitPositionInResultLabel();
	if (!highlight_position) {
		return;
	}

	resultLabel->SetTextColorAtRange(
		zaf::Color::Red(),
		zaf::TextRange{ *highlight_position, 1 });
}


std::optional<std::size_t> NonBinaryPreviewControl::GetHighlightBitPositionInResultLabel() {

	if (!modifier_.highlight_bit || modifier_.base != 2) {
		return std::nullopt;
	}

	auto text = resultLabel->GetText();

	int current_bit = 0;
	for (std::size_t position = text.length() - 1;
		position >= 0 && position < text.length();
		--position) {

		if (text[position] == L' ') {
			continue;
		}

		if (current_bit == *modifier_.highlight_bit) {
			return position;
		}

		++current_bit;
	}

	return std::nullopt;
}
