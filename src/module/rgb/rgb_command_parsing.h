#pragma once

#include <optional>
#include "module/rgb/rgb_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::module::rgb {

std::optional<RGBCommandParseResult> ParseRGBCommand(const utility::CommandLine& command_line);

}