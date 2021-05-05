#include "module/active_path/active_path_module.h"
#include "module/active_path/active_path_command.h"
#include "module/active_path/active_path_option_parsing.h"

namespace ra::module::active_path {

std::shared_ptr<Command> ActivePathModule::Interpret(const std::wstring& command_text) {

	if (command_text.empty()) {
		return nullptr;
	}

	if (command_text.front() != L'@') {
		return nullptr;
	}

	auto option = ParseActivePathOption(command_text.substr(1));
	return std::make_shared<ActivePathCommand>(option);
}

}