#include "module/date/date_command_parsing.h"
#include <Windows.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/to_numeric.h>
#include "utility/command_line_arguments.h"

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


std::optional<DateCommandParseResult> ParseDateCommand(const std::wstring& command) {

	auto arguments = utility::CommandLineArguments::Parse(command);
	if (arguments.GetCount() <= 0) {
		return std::nullopt;
	}

	if (arguments[0] != L"date") {
		return std::nullopt;
	}

	DateCommandParseResult result;
	for (auto index : zaf::Range(1, arguments.GetCount())) {

		if (!ParseSingleArgument(arguments[index], result)) {
			return std::nullopt;
		}
	}

	return result;
}

}