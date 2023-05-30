#include "utility/text_utility.h"

namespace ra::utility {

LineBreak DeterminateLineBreak(std::wstring_view text) {

	auto first_char_index = text.find_first_of(L"\r\n");
	if (first_char_index == std::wstring_view::npos) {
		return LineBreak::CRLF;
	}

	if (text[first_char_index] == L'\n') {
		return LineBreak::LF;
	}

	if (first_char_index < text.length() - 1) {
		if (text[first_char_index + 1] == L'\n') {
			return LineBreak::CRLF;
		}
	}

	return LineBreak::CR;
}


bool HasLineBreak(std::wstring_view text) {

	for (auto each_ch : text) {
		if (each_ch == L'\r' || each_ch == L'\n') {
			return true;
		}
	}
	return false;
}


std::wstring RemoveLineBreaks(std::wstring_view text) {

	constexpr wchar_t LineBreakSymbol = L'\u23CE';

	std::wstring result;

	auto current = text.begin();
	while (current < text.end()) {

		if (*current == L'\r') {

			result += LineBreakSymbol;
			++current;

			if ((current < text.end()) && (*current == L'\n')) {
				++current;
			}
		}
		else if (*current == L'\n') {
			result += LineBreakSymbol;
			++current;
		}
		else {
			result += *current;
			++current;
		}
	}

	return result;
}

}