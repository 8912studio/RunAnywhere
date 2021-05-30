#include "module/date/date_preview_control.h"
#include <zaf/reflection/reflection_type_definition.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/timer.h>

using namespace std::literals;

namespace ra::module::date {

ZAF_DEFINE_REFLECTION_TYPE(DatePreviewControl)
ZAF_DEFINE_RESOURCE_URI(L"res:///module/date/date_preview_control.xaml")
ZAF_DEFINE_END


DatePreviewControl::DatePreviewControl() {

}


DatePreviewControl::DatePreviewControl(const DateCommandParseResult& result) : 
	parse_result_(result) {

}


void DatePreviewControl::AfterParsing() {

	__super::AfterParsing();

	if (parse_result_.value) {
		time_value_ = *parse_result_.value;
	}
	else {
		time_value_ = std::time(nullptr);
	}

	UpdateLabel();

	//Start a timer to refresh current date time.
	StartTimerIfNeeded();
}


void DatePreviewControl::StartTimerIfNeeded() {

	if (parse_result_.value) {
		return;
	}

	Subscriptions() +=
		zaf::rx::Interval(1s, zaf::Scheduler::Main()).Subscribe([this](int) {

			time_value_ = std::time(nullptr);
			UpdateLabel();
		});
}


void DatePreviewControl::UpdateLabel() {

	auto time_text = GenerateTimeText();
	label->SetText(time_text);
}


std::wstring DatePreviewControl::GenerateTimeText() const {

	if (parse_result_.output_raw_value) {
		return std::to_wstring(time_value_);
	}

	std::tm tm;
	auto error = localtime_s(&tm, &time_value_);
	if (error) {
		return L"Invalid value";
	}

	std::wostringstream stream;
	stream.imbue(std::locale(""));
	stream << std::put_time(&tm, L"%x %A %X %z");
	return stream.str();
}


std::wstring DatePreviewControl::GetText() const {
	return label->GetText();
}

}