#include "module/active_path/active_path_command.h"
#include "module/active_path/active_path_modifying.h"
#include "utility/clipboard.h"

namespace ra::module::active_path {

CommandBrief ActivePathCommand::Brief() {
	return CommandBrief{ std::wstring(1, PrefixChar), L"Show active path" };
}


ActivePathCommand::ActivePathCommand(const ActivePathOption& option) : option_(option) {

}


CommandBrief ActivePathCommand::GetBrief() {
	return Brief();
}


help::content::Content ActivePathCommand::GetHelpContent() {

	help::content::Content result;

	result.AddTitleLine(L"@ command");
	result.AddBodyLine(L"Show activate path.");

	result.AddTitleLine(L"Usage");
	result.AddBodyLine(L"@[suffixes]");
	result.AddBodyLine(
		L"Append different suffixes or combined suffixes to `@` to change its result.");

	result.AddTitleLine(L"Suffixes");
	result.AddTwoPartsLine(
		L"~", 
		L"Show workspace path. It must be the first suffix if it is present.");
	result.AddTwoPartsLine(L".", L"Show directory path.");
	result.AddTwoPartsLine(L"..", L"Show parent directory path.");
	result.AddTwoPartsLine(
		L"...",
		L"Show ancestor directory path. More dot it has, more levels are backed.");
	result.AddTwoPartsLine(L"n", L"Show name only. It must be the last suffix if it is present.");

	return result;
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


void ActivePathCommand::Execute() {

	utility::SetStringToClipboard(GetPreviewText());
}

}