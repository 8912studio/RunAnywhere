#pragma once

#include <optional>
#include <string_view>
#include "module/active_path/active_path_option.h"

namespace ra::mod::active_path {

ActivePathOption ParseActivePathOption(std::wstring_view text);

std::optional<ActivePathOption> TryToParseActivePathArgument(std::wstring_view argument);

}