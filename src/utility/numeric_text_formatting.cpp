#include "utility/numeric_text_formatting.h"
#include <optional>

namespace ra::utility {
namespace {

void InsertSeperatorToText(
	std::wstring& text,
	wchar_t seperator,
	std::optional<wchar_t> begin_char,
	std::size_t digit_count) {

	std::reverse(text.begin(), text.end());

	std::size_t position{};
	if (begin_char.has_value()) {

		auto begin_char_position = text.find(L'.');
		if (begin_char_position != std::wstring::npos) {
			position = begin_char_position + 1;
		}
	}

	std::size_t current_digit_count{ 0 };
	while (position < text.length()) {

		++current_digit_count;

		if (current_digit_count == digit_count + 1) {

			text.insert(position, 1, seperator);
			current_digit_count = 1;
			position += 2;
		}
		else {

			++position;
		}
	}

	std::reverse(text.begin(), text.end());
}

}


void InsertSeperatorToNumericText(std::wstring& text, int base, wchar_t seperator) {

	if (base == 2 || base == 16) {
		InsertSeperatorToText(text, seperator, std::nullopt, 8);
	}
	else if (base == 10) {
		InsertSeperatorToText(text, seperator, L'.', 3);
	}
}

}