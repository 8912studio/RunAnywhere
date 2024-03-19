#include "module/tool/date/date_preview_control.h"
#include <sstream>
#include <zaf/object/type_definition.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/timer.h>

using namespace std::literals;

namespace ra::mod::tool::date {
namespace {

constexpr float NormalStyleHeight = 90;
constexpr float HistoricalStyleHeight = 28;

std::time_t AdjustTimeWithTimeT(std::time_t value, const DateTimeAdjustment& adjustment) {

	int adjusted_value = adjustment.adjustment;
	switch (adjustment.unit) {
	case DateTimeUnit::Week:
		adjusted_value *= 7;
	case DateTimeUnit::Day:
		adjusted_value *= 24;
	case DateTimeUnit::Hour:
		adjusted_value *= 60;
	case DateTimeUnit::Minute:
		adjusted_value *= 60;
	default:
		break;
	}
	return value + adjusted_value;
}


std::optional<std::time_t> AdjustTimeWithTM(
	std::time_t value, 
	const DateTimeAdjustment& adjustment) {

	std::tm tm{};
	auto error = localtime_s(&tm, &value);
	if (error) {
		return std::nullopt;
	}

	if (adjustment.unit == DateTimeUnit::Year) {
		tm.tm_year += adjustment.adjustment;
	}
	else if (adjustment.unit == DateTimeUnit::Month) {
		tm.tm_mon += adjustment.adjustment;
	}

	auto result = mktime(&tm);
	if (result >= 0) {
		return result;
	}
	return std::nullopt;
}


std::optional<std::time_t> AdjustTime(
	std::time_t value, 
	const DateCommandParseResult& parse_result) {

	auto result = value;
	for (const auto& each_adjustment : parse_result.adjustments) {

		if (each_adjustment.unit == DateTimeUnit::Year ||
			each_adjustment.unit == DateTimeUnit::Month) {

			auto adjust_result = AdjustTimeWithTM(result, each_adjustment);
			if (!adjust_result) {
				return std::nullopt;
			}

			result = *adjust_result;
		}
		else {
			result = AdjustTimeWithTimeT(result, each_adjustment);
		}
	}
	return result;
}

}

ZAF_DEFINE_TYPE(DatePreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/date/date_preview_control.xaml")
ZAF_DEFINE_TYPE_END


DatePreviewControl::DatePreviewControl(const DateCommandParseResult& result) : 
	parse_result_(result) {

}


void DatePreviewControl::AfterParse() {

	__super::AfterParse();

	InitializeTextBox();

	if (parse_result_.value) {
		base_time_value_ = *parse_result_.value;
	}
	else {
		base_time_value_ = std::time(nullptr);
	}

	UpdateTextBox();

	//Start a timer to refresh current date time.
	StartTimerIfNeeded();
}


void DatePreviewControl::OnStyleChanged() {

	textBox->Display(Style());

	this->SetFixedHeight(
		Style() == CommandDisplayStyle::Preserved ?
		HistoricalStyleHeight : 
		NormalStyleHeight);
}


void DatePreviewControl::InitializeTextBox() {

	Subscriptions() += textBox->SelectionChangedEvent().Subscribe(
		[this](const zaf::rich_edit::SelectionChangedInfo&) {
	
		auto selection_range = textBox->GetSelectionRange();
		if (selection_range.length <= 0) {
			StartTimerIfNeeded();
		}
		else {
			timer_subscription_.reset();
		}
	});
}


void DatePreviewControl::StartTimerIfNeeded() {

	if (parse_result_.value) {
		return;
	}

	if (timer_subscription_.has_value()) {
		return;
	}

	timer_subscription_ = zaf::rx::Interval(1s, zaf::Scheduler::Main()).Subscribe([this](int) {

		base_time_value_ = std::time(nullptr);
		UpdateTextBox();
	});
}


void DatePreviewControl::UpdateTextBox() {

	auto selection_range = textBox->GetSelectionRange();

	auto time_text = GenerateTimeText();
	textBox->SetText(time_text);
	textBox->SetSelectionRange(selection_range);
}


std::wstring DatePreviewControl::GenerateTimeText() const {

	auto adjusted_time = AdjustTime(base_time_value_, parse_result_);
	if (!adjusted_time) {
		return L"Invalid value";
	}

	if (parse_result_.output_raw_value) {
		return std::to_wstring(*adjusted_time);
	}

	std::tm tm{};
	auto error = localtime_s(&tm, &*adjusted_time);
	if (error) {
		return L"Invalid value";
	}

	std::wostringstream stream;
	stream.imbue(std::locale(""));
	stream << std::put_time(&tm, L"%x %A %X %z");
	return stream.str();
}


std::wstring DatePreviewControl::GetText() const {
	return textBox->Text();
}

}