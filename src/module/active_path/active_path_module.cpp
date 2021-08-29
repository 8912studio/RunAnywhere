#include "module/active_path/active_path_module.h"
#include "module/active_path/active_path_command.h"
#include "module/active_path/active_path_option_parsing.h"

namespace ra::module::active_path {

std::shared_ptr<Command> ActivePathModule::Interpret(const utility::CommandLine& command_line) {

	const auto& command_line_text = command_line.Text();
	if (command_line_text.empty()) {
		return nullptr;
	}

	if (command_line_text.empty() != L'@') {
		return nullptr;
	}

	auto option = ParseActivePathOption(command_line_text.substr(1));
	return std::make_shared<ActivePathCommand>(option);
}

}