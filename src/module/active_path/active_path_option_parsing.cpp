#include "module/active_path/active_path_option_parsing.h"

namespace ra::mod::active_path {
namespace {

std::size_t ParseUseWorkspace(std::wstring_view text, ActivePathOption& option) {

	if (text.empty() || text.front() != L'~') {
		return 0;
	}

	option.use_workspace_path = true;
	return 1;
}


std::size_t ParseBackwardLevel(std::wstring_view text, ActivePathOption& option) {

	for (auto each_char : text) {

		if (each_char == L'.') {
			option.backward_level++;
		}
		else {
			break;
		}
	}

	//Parsed length is equal to backward level.
	return option.backward_level;
}


std::size_t ParseUseName(std::wstring_view text, ActivePathOption& option) {

	if (text.empty() || text.front() != L'n') {
		return 0;
	}

	option.use_name = true;
	return 1;
}

}

ActivePathOption ParseActivePathOption(std::wstring_view text) {

	ActivePathOption result;

	std::size_t parsed_length = ParseUseWorkspace(text, result);
	parsed_length += ParseBackwardLevel(text.substr(parsed_length), result);
	parsed_length += ParseUseName(text.substr(parsed_length), result);

	return result;
}

}