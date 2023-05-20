#include "utility/text_utility.h"

namespace ra::utility {

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