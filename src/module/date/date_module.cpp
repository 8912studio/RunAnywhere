#include "module/date/date_module.h"
#include "module/date/date_command.h"
#include "module/date/date_command_parsing.h"

namespace ra::module::date {

std::shared_ptr<Command> DateModule::Interpret(const utility::CommandLine& command_line) {

	auto parse_result = ParseDateCommand(command_line);
	if (!parse_result) {
		return nullptr;
	}

	return std::make_shared<DateCommand>(*parse_result);
}

}