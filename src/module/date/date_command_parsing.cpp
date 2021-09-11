#include "module/date/date_command_parsing.h"
#include <Windows.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/to_numeric.h>

namespace ra::module::date {
namespace {

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


std::optional<DateCommandParseResult> ParseDateCommand(const utility::CommandLine& command_line) {

	if (command_line.Command() != DateCommandLiteral) {
		return std::nullopt;
	}

	DateCommandParseResult result;
	for (const auto& each_argument : command_line.Arguments()) {

		if (!ParseSingleArgument(each_argument, result)) {
			return std::nullopt;
		}
	}

	return result;
}

}