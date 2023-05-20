#pragma once

#include <string_view>

namespace ra::utility {

bool HasLineBreak(std::wstring_view text);
std::wstring RemoveLineBreaks(std::wstring_view text);

}