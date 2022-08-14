#include "module/user_defined/parse/entry_command_parameter_parsing.h"

namespace ra::module::user_defined {
namespace {

EntryCommandParameterPart ParseSingleParameter(std::wstring_view parameter, std::size_t position) {

	EntryCommandParameterPart result;
	result.type = EntryCommandParameterPart::Type::Unknown;
	result.position = position;
	result.length = 1;

	std::size_t option_length{};

	for (auto current_position = position + 1;
		 current_position < parameter.length();
		 ++current_position) {

		auto current_char = parameter[current_position];

		if ((L'1' <= current_char && current_char <= '9')) {

			if (option_length == 0) {

				result.type = EntryCommandParameterPart::Type::General;
				result.length = current_position - position + 1;
				result.general_index = current_char - L'1' + 1;
			}
			break;
		}

		option_length++;
	}

	return result;
}


bool IsParameterQuoted(
	std::wstring_view parameter_string, 
	const EntryCommandParameterPart& parameter) {

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


std::vector<EntryCommandParameterPart> ParseEntryCommandParameter(std::wstring_view parameter_string) {

	std::vector<EntryCommandParameterPart> result;

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

}