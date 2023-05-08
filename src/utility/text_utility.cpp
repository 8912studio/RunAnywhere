#include "utility/text_utility.h"

namespace ra::utility {

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