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

	const auto& pieces = command_line.AllPieces();
	if (pieces.empty()) {
		return nullptr;
	}

	const auto& first_piece = pieces.front();
	if (first_piece.Type() != utility::CommandLinePieceType::NormalText) {
		return nullptr;
	}

	const auto& content = first_piece.Content();
	if (content.empty()) {
		return nullptr;
	}

	if (content.front() != ActivePathCommand::PrefixChar) {
		return nullptr;
	}

	auto option = ParseActivePathOption(content.substr(1));
	return std::make_unique<ActivePathCommand>(option);
}

}