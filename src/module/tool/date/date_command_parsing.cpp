#include "module/tool/date/date_command_parsing.h"
#include <zaf/base/string/to_numeric.h>
#include "module/common/parse_status.h"

namespace ra::mod::tool::date {
namespace {

ParseStatus ParseSwitch(const std::wstring& argument, DateCommandParseResult& result) {

	if (argument[0] != L'/') {
		return ParseStatus::Mismatched;
	}

	if (argument.size() <= 1) {
		return ParseStatus::OK;
	}

	if (argument[1] == L'e') {
		result.output_raw_value = true;
		return ParseStatus::OK;
	}

	return ParseStatus::OK;
}


ParseStatus ParseAdjustment(std::wstring_view argument, DateCommandParseResult& result) {

	if (argument[0] != L'+' && argument[0] != L'-') {
		return ParseStatus::Mismatched;
	}

	//There is only a sign char, ignore the argument.
	if (argument.size() == 1) {
		return ParseStatus::OK;
	}

	auto last_digit_index = argument.find_last_of(L"0123456789");
	if (last_digit_index == std::wstring::npos) {
		return ParseStatus::Error;
	}

	auto number_string = argument.substr(0, last_digit_index + 1);
	int number{};
	if (!zaf::TryToNumeric<int>(std::wstring{ number_string }, number)) {
		return ParseStatus::Error;
	}

	DateTimeUnit unit = DateTimeUnit::Second;
	auto unit_string = argument.substr(last_digit_index + 1);
	if (!unit_string.empty()) {
		auto parsed_unit = DateTimeUnitFromString(unit_string);
		if (!parsed_unit) {
			return ParseStatus::Error;
		}
		unit = *parsed_unit;
	}

	DateTimeAdjustment adjustment;
	adjustment.unit = unit;
	adjustment.adjustment = number;
	result.adjustments.push_back(adjustment);
	return ParseStatus::OK;
}


bool ParseSingleArgument(
	const utility::CommandLinePiece& argument, 
	DateCommandParseResult& result) {

	if (argument.Type() == utility::CommandLinePieceType::NormalText) {
		auto parse_status = ParseSwitch(argument.Content(), result);
		if (parse_status == ParseStatus::Error) {
			return false;
		}
		if (parse_status == ParseStatus::OK) {
			return true;
		}
	}

	auto parse_status = ParseAdjustment(argument.Content(), result);
	if (parse_status == ParseStatus::Error) {
		return false;
	}

	if (parse_status == ParseStatus::OK) {
		return true;
	}

	std::time_t value{};
	if (zaf::TryToNumeric(argument.Content(), value)) {
		result.value = value;
		return true;
	}

	return false;
}

}

std::optional<DateCommandParseResult> Parse(const utility::CommandLine& command_line) {

	DateCommandParseResult result;
	for (const auto& each_argument : command_line.Arguments()) {

		if (!ParseSingleArgument(each_argument, result)) {
			return std::nullopt;
		}
	}

	return result;
}

}