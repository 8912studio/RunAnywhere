#include "module/user_defined/command_parameter_parsing.h"

namespace {

CommandParameter ParseSingleParameter(std::wstring_view parameter, std::size_t position) {

	std::size_t backward_level{};

	auto current_position = position + 1;
	while (current_position < parameter.size() && parameter[current_position] == L'.') {
		++backward_level;
		++current_position;
	}

	CommandParameter result;
	result.position = position;

	auto current_char = parameter[current_position];
	if (current_char == L'@') {

		result.type = CommandParameter::Type::ActivePath;
		result.length = current_position - position + 1;
		result.backward_level = backward_level;
	}
	else if ((L'1' <= current_char && current_char <= '9') && (backward_level == 0)) {

		result.type = CommandParameter::Type::General;
		result.length = current_position - position + 1;
		result.general_index = current_char - L'1' + 1;
	}
	else {

		result.type = CommandParameter::Type::Unknown;
		result.length = 1;
	}
	
	return result;
}


bool IsParameterQuoted(std::wstring_view parameter_string, const CommandParameter& parameter) {

	if (parameter.position <= 0) {
		return false;
	}

	auto end_position = parameter.position + parameter.length;
	if (end_position >= parameter_string.size()) {
		return false;
	}

	return
		parameter_string[parameter.position - 1] == L'"' &&
		parameter_string[end_position] == L'"';
}

}


std::vector<CommandParameter> ParseParameter(std::wstring_view parameter_string) {

	std::vector<CommandParameter> result;

	std::size_t current_position = 0;
	std::size_t last_quote_position = std::wstring_view::npos;
	while (current_position < parameter_string.size()) {

		current_position = parameter_string.find(L'%', current_position);
		if (current_position == std::wstring_view::npos) {
			break;
		}

		auto parsed_parameter = ParseSingleParameter(parameter_string, current_position);
		current_position += parsed_parameter.length;

		if (parsed_parameter.position > 0 && parsed_parameter.position - 1 != last_quote_position) {

			parsed_parameter.is_quoted = IsParameterQuoted(parameter_string, parsed_parameter);
			if (parsed_parameter.is_quoted) {

				last_quote_position = current_position;
				++current_position;
			}
		}

		result.push_back(std::move(parsed_parameter));
	}

	return result;
}