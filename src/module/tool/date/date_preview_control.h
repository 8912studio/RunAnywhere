#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include "module/command_preview_control.h"
#include "module/tool/date/date_command_parse_result.h"
#include "utility/preview_text_box.h"

namespace ra::mod::tool::date {

class DatePreviewControl : public CommandPreviewControl {
public:
	ZAF_DECLARE_TYPE;

	explicit DatePreviewControl(const DateCommandParseResult& result);

	std::wstring GetText() const;
	 
protected:
	void AfterParse() override;
	void OnStyleChanged() override;

private:
	void InitializeTextBox();
	void StartTimerIfNeeded();
	void UpdateTextBox();
	std::wstring GenerateTimeText() const;

private:
	ZAF_BIND_CONTROL(utility::PreviewTextBox, textBox);

	DateCommandParseResult parse_result_;
	std::optional<zaf::Subscription> timer_subscription_;
	std::time_t time_value_{};
};

}