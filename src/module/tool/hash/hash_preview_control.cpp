#include "module/tool/hash/hash_preview_control.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/rx/scheduler.h>
#include "module/common/error_messages.h"
#include "module/tool/hash/hash_calculating.h"

namespace ra::mod::tool::hash {
namespace {

template<bool IsHistorical>
struct StyleMetrics;

template<>
struct StyleMetrics<false> {
	static constexpr auto ProgressHeight = 90;
	static constexpr auto ProgressCircleAlignment = zaf::AxisAlignment::Center;
	static constexpr float ProgressCircleThickness = 7;
	static zaf::Frame ProgressCirclePadding() {
		return zaf::Frame{ 0, 20, 0, 20 };
	}
};

template<>
struct StyleMetrics<true> {
	static constexpr auto ProgressHeight = 28;
	static constexpr auto ProgressCircleAlignment = zaf::AxisAlignment::Start;
	static constexpr float ProgressCircleThickness = 3;
	static zaf::Frame ProgressCirclePadding() {
		return zaf::Frame{ 0, 4, 0, 4 };
	}
};

}

ZAF_OBJECT_IMPL(HashPreviewControl);

HashPreviewControl::HashPreviewControl(HashAlgorithmInfo algorithm_info) :
	hash_algorithm_info_(std::move(algorithm_info)) {

}


void HashPreviewControl::AfterParse() {

	__super::AfterParse();

	ShowAlgorithmLabel();

	TextDisplayMode display_mode;
	display_mode.use_fixed_width_font = true;
	display_mode.word_wrapping = zaf::WordWrapping::Character;
	hashResultControl->SetDisplayMode(display_mode);
}


void HashPreviewControl::ShowAlgorithmLabel() {

	if (hash_algorithm_info_.displayed_name.empty()) {
		algorithmLabel->SetIsVisible(false);
		return;
	}

	algorithmLabel->SetText(hash_algorithm_info_.displayed_name);
	algorithmLabel->SetTooltip(hash_algorithm_info_.tooltip);
	algorithmLabel->SetIsVisible(true);
}


void HashPreviewControl::OnStyleChanged() {

	auto update_guard = BeginUpdate();

	hashResultControl->ChangeStyle(Style());
	errorView->ChangeStyle(Style());
	AdjustControlStyles();
}


void HashPreviewControl::AdjustControlStyles() {

	auto set_style = [this](auto metrics) {
		progressCircle->SetFixedHeight(metrics.ProgressHeight);
		progressCircle->SetPadding(metrics.ProgressCirclePadding());
		progressCircle->SetAxisAlignment(metrics.ProgressCircleAlignment);
		progressCircle->SetThickness(metrics.ProgressCircleThickness);
	};

	if (Style() == CommandDisplayStyle::Preserved) {
		set_style(StyleMetrics<true>{});
	}
	else {
		set_style(StyleMetrics<false>{});
	}
}


void HashPreviewControl::ChangeLayout(LayoutType type) {

	progressCircle->SetIsVisible(false);
	errorView->SetIsVisible(false);
	hashResultControl->SetIsVisible(false);

	switch (type) {
	case LayoutType::Progress:
		progressCircle->SetIsVisible(true);
		break;
	case LayoutType::Error:
		errorView->ShowErrorText(ErrorMessages::UnableToReadFile);
		errorView->SetIsVisible(true);
		break;
	case LayoutType::Result:
		hashResultControl->SetIsVisible(true);
		break;
	default:
		break;
	}
}


void HashPreviewControl::ShowHash(const GeneralInput& input) {

	if (auto file_path = input.GetFile()) {
		ShowFileHash(*file_path);
	}
	else if (auto text = input.GetText()) {
		ShowStringHash(text->content, text->encoding);
	}
}


void HashPreviewControl::ShowFileHash(const std::filesystem::path& file_path) {

	contentStatusBar->ShowFile(file_path);
	contentStatusBar->SetIconTooltip(L"Input is file");

	ChangeLayout(LayoutType::Progress);

	Subscriptions() += CalculateFileHash(file_path, hash_algorithm_info_.algorithm_creator)
		.ObserveOn(zaf::Scheduler::Main())
		.Subscribe([this](const HashResult& hash_result) {

		if (hash_result.result.empty()) {

			progressCircle->SetMaxValue(hash_result.total_size);
			progressCircle->SetValue(hash_result.current_size);
		}
		else {

			SetHashText(hash_result.result);
			ChangeLayout(LayoutType::Result);
		}
	}, 
	[this](const std::exception_ptr&) {
		ChangeLayout(LayoutType::Error);
	}, 
	[this]() {
		calculate_finished_event_.AsObserver().OnNext({});
		calculate_finished_event_.AsObserver().OnCompleted();
	});
}


void HashPreviewControl::ShowStringHash(const std::wstring& string, TextEncoding encoding) {

	contentStatusBar->ShowText(string, encoding);
	contentStatusBar->SetIconTooltip(L"Input is text");
	contentStatusBar->SetEncodingTooltip(
		encoding == TextEncoding::UTF8 ? L"Input text is UTF-8" : L"Input text is UTF-16");

	auto hash = CalculateStringHash(string, encoding, hash_algorithm_info_.algorithm_creator);
	SetHashText(hash);

	ChangeLayout(LayoutType::Result);
}


void HashPreviewControl::SetHashText(const std::wstring& hash) {

	if (use_upper_case_) {
		hashResultControl->SetText(zaf::ToUppercased(hash));
	}
	else {
		hashResultControl->SetText(hash);
	}
}


std::wstring HashPreviewControl::GetText() {
	return hashResultControl->GetText();
}


zaf::Frame HashPreviewControl::GetExpectedMargin() {
	return {};
}

}