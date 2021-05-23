#pragma once

#include <optional>
#include <string_view>
#include "module/calculator/parse/parse_result.h"

namespace ra::module::calculator {

std::optional<ParseResult> ParseCommand(std::wstring_view command);

}