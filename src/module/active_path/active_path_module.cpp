#include "module/active_path/active_path_module.h"
#include "module/active_path/active_path_command.h"
#include "module/active_path/active_path_option_parsing.h"

namespace ra::mod::active_path {

std::vector<CommandBrief> ActivePathModule::QuerySuggestedCommands(
	const std::wstring& command_text) {

	if (command_text.empty() || command_text.front() == ActivePathCommand::PrefixChar) {
		return { ActivePathCommand::Brief() };
	}

	return {};
}


std::unique_ptr<Command> ActivePathModule::CreateCommand(
	const utility::CommandLine& command_line) {

	const auto& command_line_text = command_line.RawText();
	if (command_line_text.empty()) {
		return nullptr;
	}

	if (command_line_text.front() != ActivePathCommand::PrefixChar) {
		return nullptr;
	}

	auto option = ParseActivePathOption(command_line_text.substr(1));
	return std::make_unique<ActivePathCommand>(option);
}

}