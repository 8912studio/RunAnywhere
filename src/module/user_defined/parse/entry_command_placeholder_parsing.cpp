#include "module/user_defined/parse/entry_command_placeholder_parsing.h"
#include <optional>

namespace ra::module::user_defined {
namespace {
 
std::optional<EntryCommandPlaceholder> ParseSinglePlaceholder(
	std::wstring_view input,
	std::size_t& position) {

	auto begin_position = position;

	position++;
	if (position >= input.size()) {
		return std::nullopt;
	}

	auto current_char = input[position];
	if (current_char < L'1' || L'9' < current_char) {
		return std::nullopt;
	}

	position++;

	EntryCommandPlaceholder result;
	result.position = begin_position;
	result.length = position - begin_position;
	result.index = current_char - L'1' + 1;
	return result;
}

}


std::vector<EntryCommandPlaceholder> ParseEntryCommandPlaceholders(std::wstring_view input) {

	std::vector<EntryCommandPlaceholder> result;

	std::size_t current_position = 0;
	while (current_position < input.size()) {

		current_position = input.find(L'%', current_position);
		if (current_position == std::wstring_view::npos) {
			break;
		}

		auto placeholder = ParseSinglePlaceholder(input, current_position);
		if (placeholder) {
			result.push_back(*placeholder);
		}
	}

	return result;
}

}