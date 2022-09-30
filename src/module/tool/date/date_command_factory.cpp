#include "module/tool/date/date_command_factory.h"
#include <zaf/base/string/to_numeric.h>
#include "module/tool/date/date_command.h"

namespace ra::module::tool::date {
namespace {

CommandBrief GetDateCommandBrief() {
    return CommandBrief{
        L"date",
        L"Convert between date string and UNIX timestamp"
    };
}


bool ParseSwitch(const std::wstring& argument, DateCommandParseResult& result) {

	if (argument[0] != L'/') {
		return false;
	}

	if (argument.size() <= 1) {
		return true;
	}

	if (argument[1] == L'e') {
		result.output_raw_value = true;
		return true;
	}

	return true;
}


bool ParseSingleArgument(const std::wstring& argument, DateCommandParseResult& result) {

	if (ParseSwitch(argument, result)) {
		return true;
	}

	std::time_t value{};
	if (zaf::TryToNumeric(argument, value)) {
		result.value = value;
		return true;
	}

	return false;
}

}


std::optional<DateCommandParseResult> DateCommandFactory::Parse(
    const utility::CommandLine& command_line) {

	DateCommandParseResult result;
    for (const auto& each_argument : command_line.Arguments()) {

        if (!ParseSingleArgument(each_argument, result)) {
            return std::nullopt;
        }
    }

    return result;
}


DateCommandFactory::DateCommandFactory() : ToolCommandFactory(GetDateCommandBrief()) {

}


std::shared_ptr<Command> DateCommandFactory::CreateCommand(
    const utility::CommandLine& command_line) {

	auto parse_result = Parse(command_line);
	if (!parse_result) {
		return nullptr;
	}

	return std::make_shared<DateCommand>(*parse_result);
}

}