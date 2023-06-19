#include "module/calculator/parse/parse_context.h"

namespace ra::mod::calculator {

ParseContext::ParseContext(std::wstring_view command_text) : 
	command_text_(command_text.data(), command_text.length()) {

}


std::optional<wchar_t> ParseContext::GetCharAtOffset(int offset) const {

	auto new_index = static_cast<int>(current_index_) + offset;
	if (new_index < 0 || new_index > static_cast<int>(command_text_.length())) {
		return std::nullopt;
	}

	return command_text_[new_index];
}


void ParseContext::Forward(std::size_t length) {

	std::size_t new_index = current_index_ + length;

	if (new_index > command_text_.length()) {
		new_index = command_text_.length();
	}

	last_parsed_length_ = new_index - current_index_;
	current_index_ = new_index;
}


void ParseContext::SkipWhiteSpaces() {
	BeginRead().SkipWhiteSpaces();
}


ParseReader ParseContext::BeginRead() {
	return ParseReader{ *this };
}

}