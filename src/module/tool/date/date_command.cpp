#include "module/tool/date/date_command.h"
#include <zaf/creation.h>
#include "help/built_in_help_content_manager.h"
#include "module/common/command_error_control.h"
#include "module/common/copy_executor.h"
#include "module/tool/date/date_command_parsing.h"

namespace ra::mod::tool::date { 

std::wstring DateCommand::Keyword() {
	return L"date";
}


std::wstring DateCommand::GetKeyword() {
	return Keyword();
}


help::HelpContent DateCommand::GetHelpContent() {
	return {
		Keyword(),
		help::BuiltInHelpContentManager::Instance().GetDetail(Keyword())
	};
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