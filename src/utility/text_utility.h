#pragma once

#include <string_view>

namespace ra::utility {

enum class LineBreak {
    CRLF,
    CR,
    LF,
};

class LineBreakInfo {
public:
    LineBreak first_line_break{ LineBreak::CRLF };
    bool all_the_same{};
};

LineBreakInfo DeterminateLineBreakInfo(std::wstring_view text);

LineBreak DeterminateLineBreak(std::wstring_view text);

bool HasLineBreak(std::wstring_view text);
std::wstring ReplaceWhitespacesToVisibleChars(std::wstring_view text);

}