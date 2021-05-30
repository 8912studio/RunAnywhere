#include "module/date/date_command.h"
#include <zaf/creation.h>
#include "utility/clipboard.h"

namespace ra::module::date { 

DateCommand::DateCommand(const DateCommandParseResult& parse_result) : parse_result_(parse_result) {

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