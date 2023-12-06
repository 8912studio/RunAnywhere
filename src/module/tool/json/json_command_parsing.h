#pragma once

#include "module/tool/json/json_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::mod::tool::json {

JSONCommandParseResult ParseJSONCommand(const utility::CommandLine& command_line);

}