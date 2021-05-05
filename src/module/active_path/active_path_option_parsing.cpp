#include "module/active_path/active_path_option_parsing.h"

namespace ra::module::active_path {
namespace {

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


std::size_t ParseNameOnly(std::wstring_view text, ActivePathOption& option) {

	if (text.empty() || text.front() != L'n') {
		return 0;
	}

	option.name_only = true;
	return 1;
}

}

ActivePathOption ParseActivePathOption(std::wstring_view text) {

	ActivePathOption result;

	std::size_t backward_level_length = ParseBackwardLevel(text, result);

	ParseNameOnly(text.substr(backward_level_length), result);

	return result;
}

}