#pragma once

#include "module/tool/hash/hash_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::mod::tool::hash {

HashCommandParseResult ParseHashCommand(const utility::CommandLine& command_line);

}