#pragma once

#include <optional>
#include "module/rgb/rgb_command_parse_result.h"

namespace ra::module::rgb {

std::optional<RGBCommandParseResult> ParseRGBCommand(const std::wstring& command_text);

}