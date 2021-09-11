#include "module/date/date_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::module::date { 

DateCommand::DateCommand(const DateCommandParseResult& parse_result) : parse_result_(parse_result) {

}


help::content::Content DateCommand::GetHelpContent() {

	help::content::Content result;
	result.AddTitleLine(L"Date command");
	result.AddBodyLine(L"Convert between human readable date time string and UNIX timestamp.");

	result.AddTitleLine(L"Usage");
	result.AddBodyLine(L"date [timestamp] [switches]");
	result.AddBodyLine(L"If `timestamp` is omitted, current date time would be used.");

	result.AddTitleLine(L"Switches");
	result.AddTwoPartsLine(L"/e", L"Show the result as UNIX timestamp.");

	return result;
}


std::shared_ptr<CommandPreviewControl> DateCommand::GetPreviewControl() {

	if (!preview_control_) {
		preview_control_ = zaf::Create<DatePreviewControl>(parse_result_);
	}

	return preview_control_;
}


void DateCommand::Execute() {

	if (!preview_control_) {
		return;
	}

	utility::SetStringToClipboard(preview_control_->GetText());
}

}