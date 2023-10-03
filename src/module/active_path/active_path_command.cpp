#include "module/active_path/active_path_command.h"
#include "help/built_in_help_content_manager.h"
#include "module/active_path/active_path_modifying.h"
#include "module/common/copy_executor.h"

namespace ra::mod::active_path {

ActivePathCommand::ActivePathCommand(const ActivePathOption& option) : option_(option) {

}


std::wstring ActivePathCommand::GetKeyword() {
	return std::wstring(1, PrefixChar);
}


help::HelpContent ActivePathCommand::GetHelpContent() {
	return {
		HelpContentIdentity,
		help::BuiltInHelpContentManager::Instance().GetDetail(HelpContentIdentity)
	};
}


bool ActivePathCommand::Interpret(
	const utility::CommandLine& command_line,
	const context::DesktopContext& desktop_context,
	bool is_reusing) {

	//Not allow to reuse.
	if (is_reusing) {
		return false;
	}

	desktop_context_ = desktop_context;
	return true;
}


std::wstring ActivePathCommand::GetPreviewText() {

	auto new_active_path = ModifyActivePathByOption(desktop_context_.active_path, option_);
	return new_active_path.GetPath().wstring();
}


std::shared_ptr<CommandExecutor> ActivePathCommand::GetExecutor() {
	return CopyExecutor::TryCreate(GetPreviewText());
}

}