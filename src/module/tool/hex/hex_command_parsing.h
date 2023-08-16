#pragma once

#include <optional>
#include "module/tool/hex/hex_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::mod::tool::hex {

std::optional<HexCommandParseResult> ParseHexCommand(const utility::CommandLine& command_line);

}