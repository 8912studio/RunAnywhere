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

	const auto& parts = command_line.AllParts();
	if (parts.empty()) {
		return nullptr;
	}

	const auto& first_part = parts.front();
	if (first_part.empty()) {
		return nullptr;
	}

	if (first_part.front() != ActivePathCommand::PrefixChar) {
		return nullptr;
	}

	auto option = ParseActivePathOption(first_part.substr(1));
	return std::make_unique<ActivePathCommand>(option);
}

}