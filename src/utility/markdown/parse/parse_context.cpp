#include "utility/markdown/parse/parse_context.h"

namespace ra::help::markdown::parse {

ParseContext::ParseContext(std::wstring_view input) : 
    input_(input),
    current_index_(0),
    current_pointer_(input.data()) {

}


wchar_t ParseContext::CurrentChar() const {

    if (current_index_ >= input_.length()) {
        return 0;
    }

    auto current_char = input_[current_index_];
    if (current_char == L'\r') {
        return L'\n';
    }

    return current_char;
}


std::optional<wchar_t> ParseContext::PreviousChar() const {

    if (current_index_ == 0) {
        return std::nullopt;
    }

    auto previous_char = input_[current_index_ - 1];
    if (previous_char == L'\r') {
        return L'\n';
    }

    return previous_char;
}


bool ParseContext::IsAtLineStart() const {

    auto previous_char = PreviousChar();
    if (previous_char) {
        return *previous_char == L'\n';
    }
    return true;
}


bool ParseContext::IsAtLineEnd() const {

    auto current_char = CurrentChar();
    return
        (current_char == L'\n') ||
        (current_char == L'\0');
}


bool ParseContext::Forward() {

    if (current_index_ == input_.length()) {
        return false;
    }

    auto last_char = input_[current_index_];
    current_index_++;

    if (current_index_ < input_.length()) {
        if ((last_char == L'\r') && (input_[current_index_] == L'\n')) {
            current_index_++;
        }
    }

    current_pointer_ = input_.data() + current_index_;
    return true;
}


void ParseContext::SkipSpaces() {

    do {
        if (CurrentChar() != L' ') {
            break;
        }
    }
    while (Forward());
}


bool ParseContext::ParseText(std::wstring_view text) {

    auto transaction = BeginTransaction();

    for (auto ch : text) {
        if (CurrentChar() != ch) {
            return false;
        }
        Forward();
    }

    transaction.Commit();
    return true;
}


void ParseContext::Rollback(std::size_t index) {
    current_index_ = index;
    current_pointer_ = input_.data() + current_index_;
}

}