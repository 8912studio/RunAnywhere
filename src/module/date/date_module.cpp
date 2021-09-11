#include "module/date/date_module.h"
#include "module/date/date_command.h"
#include "module/date/date_command_parsing.h"

namespace ra::module::date {

std::vector<CommandBrief> DateModule::QuerySuggestedCommands(const std::wstring& command_text) {

	std::wstring date_command{ DateCommandLiteral };
	if (date_command.find(command_text) == 0) {

		return {
			CommandBrief{ date_command, L"Convert between date string and UNIX timestamp" },
		};
	}

	return {};
}


std::shared_ptr<Command> DateModule::Interpret(const utility::CommandLine& command_line) {

	auto parse_result = ParseDateCommand(command_line);
	if (!parse_result) {
		return nullptr;
	}

	return std::make_shared<DateCommand>(*parse_result);
}

}