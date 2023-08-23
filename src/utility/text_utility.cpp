#include "utility/text_utility.h"
#include <optional>

namespace ra::utility {

LineBreakInfo DeterminateLineBreakInfo(std::wstring_view text) {

	auto find_next_line_break = [text](std::size_t& index) -> std::optional<LineBreak> {
	
		auto line_break_index = text.find_first_of(L"\r\n", index);
		if (line_break_index == std::wstring_view::npos) {
			return std::nullopt;
		}

		index = line_break_index;

		if (text[index] == L'\n') {
			++index;
			return LineBreak::LF;
		}

		if (index < text.length() - 1) {
			if (text[index + 1] == L'\n') {
				index += 2;
				return LineBreak::CRLF;
			}
		}

		++index;
		return LineBreak::CR;
	};

	std::optional<LineBreak> first_line_break;
	bool all_the_same{ true };

	std::size_t current_index{};
	while (current_index < text.length()) {

		auto line_break = find_next_line_break(current_index);
		if (!line_break) {
			break;
		}

		if (!first_line_break) {
			first_line_break = *line_break;
		}
		else if (*first_line_break != *line_break) {
			all_the_same = false;
			break;
		}
	}

	LineBreakInfo result;
	result.first_line_break = first_line_break.value_or(LineBreak::CRLF);
	result.all_the_same = all_the_same;
	return result;
}


bool HasLineBreak(std::wstring_view text) {

	for (auto each_ch : text) {
		if (each_ch == L'\r' || each_ch == L'\n') {
			return true;
		}
	}
	return false;
}


void RemoveMultipleLines(std::wstring& text) {

	auto line_break_index = text.find_first_of(L"\r\n");
	if (line_break_index != std::wstring::npos) {
		text.erase(text.begin() + line_break_index, text.end());
	}
}


std::wstring ReplaceWhitespacesToVisibleChars(std::wstring_view text) {

	constexpr wchar_t LineBreakSymbol = L'\u23CE';
	constexpr wchar_t SpaceSymbol = L'\u2423';
	constexpr wchar_t TabSymbol = L'\u21E5';

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
		else if (*current == L' ') {
			result += SpaceSymbol;
			++current;
		}
		else if (*current == L'\t') {
			result += TabSymbol;
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