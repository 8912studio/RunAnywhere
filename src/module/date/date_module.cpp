#include "module/date/date_module.h"
#include "module/date/date_command.h"
#include "module/date/date_command_parsing.h"

namespace ra::module::date {

std::shared_ptr<Command> DateModule::Interpret(const std::wstring& command_text) {

	auto parse_result = ParseDateCommand(command_text);
	if (!parse_result) {
		return nullptr;
	}

	return std::make_shared<DateCommand>(*parse_result);
}

}