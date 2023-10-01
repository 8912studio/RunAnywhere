#include "module/tool/date/date_command.h"
#include <zaf/creation.h>
#include "help/built_in_help_content_manager.h"
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


std::shared_ptr<help::markdown::element::Element> DateCommand::GetHelpContent() {
	return help::BuiltInHelpContentManager::Instance().GetDetail(Brief().Command());
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