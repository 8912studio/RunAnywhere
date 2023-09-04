#include "help/markdown/plain_string_builder.h"

namespace ra::help::markdown {

void PlainStringBuilder::Append(wchar_t ch) {

    //Remove heading spaces in line.
    if (ch == L' ') {
        if (string_.empty()) {
            if (is_first_part_) {
                return;
            }
        }
        else if (string_.back() == L'\n') {
            return;
        }
    }

    if (ch == L'\n') {

        //Remove empty lines.
        if (string_.empty()) {
            if (is_first_part_) {
                return;
            }
        }
        //Remove empty lines.
        else if (string_.back() == L'\n') {
            return;
        }

        //Remove tailing spaces in line.
        RemoveTailingSpaces();
    }

    string_.append(1, ch);
}


std::wstring PlainStringBuilder::Build(bool is_last_part) {

    if (is_last_part) {
        RemoveTailingSpaces();
    }
    return std::move(string_);
}


void PlainStringBuilder::RemoveTailingSpaces() {

    auto last_valid_index = string_.find_last_not_of(L' ');
    if (last_valid_index != std::wstring::npos) {
        string_.erase(last_valid_index + 1);
    }
}

}