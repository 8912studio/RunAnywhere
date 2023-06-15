#pragma once

#include <string_view>

namespace ra::utility {

enum class LineBreak {
    CRLF,
    CR,
    LF,
};

LineBreak DeterminateLineBreak(std::wstring_view text);

bool HasLineBreak(std::wstring_view text);
std::wstring ReplaceWhitespacesToVisibleChars(std::wstring_view text);

}