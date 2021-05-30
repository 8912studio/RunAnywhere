#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/command_preview_control.h"
#include "module/date/date_command_parse_result.h"

namespace ra::module::date {

class DatePreviewControl : public CommandPreviewControl {
public:
	ZAF_DECLARE_REFLECTION_TYPE;

	DatePreviewControl();
	DatePreviewControl(const DateCommandParseResult& result);

	std::wstring GetText() const;
	 
protected:
	void AfterParsing() override;

private:
	void StartTimerIfNeeded();
	void UpdateLabel();
	std::wstring GenerateTimeText() const;

private:
	ZAF_BIND_CONTROL(zaf::Label, label);

	DateCommandParseResult parse_result_;
	std::time_t time_value_{};
};

}