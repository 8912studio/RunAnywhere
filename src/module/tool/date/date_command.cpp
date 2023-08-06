#include "module/tool/date/date_command.h"
#include <zaf/creation.h>
#include "module/common/command_error_control.h"
#include "module/common/copy_executor.h"
#include "module/tool/date/date_command_parsing.h"

namespace ra::mod::tool::date { 

CommandBrief DateCommand::Brief() {
	return CommandBrief{
		L"date",
		L"Convert between date string and UNIX timestamp"
	};
}


CommandBrief DateCommand::GetBrief() {
	return Brief();
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


bool DateCommand::Interpret(
	const utility::CommandLine& command_line,
	const context::DesktopContext& desktop_context,
	bool is_reusing) {

	//Not allow to reuse.
	if (is_reusing) {
		return false;
	}

	parse_result_ = Parse(command_line);
	return true;
}


std::shared_ptr<CommandPreviewControl> DateCommand::GetPreviewControl() {

	if (!parse_result_) {
		return CommandErrorControl::InvalidArgument();
	}

	if (!preview_control_) {
		preview_control_ = zaf::Create<DatePreviewControl>(*parse_result_);
	}

	return preview_control_;
}


std::shared_ptr<CommandExecutor> DateCommand::GetExecutor() {

	if (!preview_control_) {
		return nullptr;
	}

	return CopyExecutor::TryCreate(preview_control_->GetText());
}

}