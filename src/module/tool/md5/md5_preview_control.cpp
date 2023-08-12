#include "module/tool/md5/md5_preview_control.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/scheduler.h>
#include "module/common/error_messages.h"
#include "module/tool/md5/md5_calculating.h"

namespace ra::mod::tool::md5 {
namespace {

template<bool IsHistorical>
struct StyleMetrics;

template<>
struct StyleMetrics<false> {
	static constexpr float FixedHeight = 110;
	static constexpr auto ProgressCircleAlignment = zaf::AxisAlignment::Center;
	static constexpr float ProgressCircleThickness = 7;
	static zaf::Frame ProgressCirclePadding() {
		return zaf::Frame{ 0, 14, 0, 14 };
	}
};

template<>
struct StyleMetrics<true> {
	static constexpr float FixedHeight = 60;
	static constexpr auto ProgressCircleAlignment = zaf::AxisAlignment::Start;
	static constexpr float ProgressCircleThickness = 3;
	static zaf::Frame ProgressCirclePadding() {
		return zaf::Frame{ 0, 4, 0, 4 };
	}
};

}

ZAF_DEFINE_TYPE(MD5PreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/md5/md5_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void MD5PreviewControl::OnStyleChanged() {

	auto update_guard = BeginUpdate();

	md5ResultControl->Display(Style());
	errorView->ChangeStyle(Style());
	AdjustControlStyles();
}


void MD5PreviewControl::AdjustControlStyles() {

	auto set_style = [this](auto metrics) {
		progressCircle->SetPadding(metrics.ProgressCirclePadding());
		progressCircle->SetAxisAlignment(metrics.ProgressCircleAlignment);
		progressCircle->SetThickness(metrics.ProgressCircleThickness);
		this->SetFixedHeight(metrics.FixedHeight);
	};

	if (Style() == CommandDisplayStyle::Preserved) {
		set_style(StyleMetrics<true>{});
	}
	else {
		set_style(StyleMetrics<false>{});
	}
}


void MD5PreviewControl::ChangeLayout(LayoutType type) {

	progressCircle->SetIsVisible(false);
	errorView->SetIsVisible(false);
	md5ResultControl->SetIsVisible(false);

	switch (type) {
	case LayoutType::Progress:
		progressCircle->SetIsVisible(true);
		break;
	case LayoutType::Error:
		errorView->ShowErrorText(ErrorMessages::UnableToReadFile);
		errorView->SetIsVisible(true);
		break;
	case LayoutType::Result:
		md5ResultControl->SetIsVisible(true);
		break;
	default:
		break;
	}
}


void MD5PreviewControl::ShowFileMD5(const std::filesystem::path& file_path) {

	contentStatusBar->ShowFile(file_path);
	contentStatusBar->SetIconTooltip(L"Input is file");

	ChangeLayout(LayoutType::Progress);

	Subscriptions() += CalculateFileMD5(file_path).ObserveOn(zaf::Scheduler::Main()).Subscribe(
		[this](const MD5Result& md5_result) {

		if (md5_result.md5.empty()) {

			progressCircle->SetMaxValue(md5_result.total_size);
			progressCircle->SetValue(md5_result.current_size);
		}
		else {

			SetMD5Text(md5_result.md5);
			ChangeLayout(LayoutType::Result);
		}
	}, 
	[this](const zaf::Error&) {
		ChangeLayout(LayoutType::Error);
	}, 
	[this]() {
		calculate_finished_event_.GetObserver().OnNext({});
		calculate_finished_event_.GetObserver().OnCompleted();
	});
}


void MD5PreviewControl::ShowStringMD5(const std::wstring& string, TextEncoding encoding) {

	contentStatusBar->ShowText(string, encoding);
	contentStatusBar->SetIconTooltip(L"Input is text");
	contentStatusBar->SetEncodingTooltip(
		encoding == TextEncoding::UTF8 ? L"Input text is UTF-8" : L"Input text is UTF-16");

	auto md5 = CalculateStringMD5(string, encoding);
	SetMD5Text(md5);

	ChangeLayout(LayoutType::Result);
}


void MD5PreviewControl::SetMD5Text(const std::wstring& md5) {

	if (use_upper_case_) {
		md5ResultControl->SetText(zaf::ToUppercased(md5));
	}
	else {
		md5ResultControl->SetText(md5);
	}
}


std::wstring MD5PreviewControl::GetText() {
	return md5ResultControl->Text();
}

}