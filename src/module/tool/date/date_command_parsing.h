#pragma once

#include "module/tool/date/date_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::mod::tool::date {

std::optional<DateCommandParseResult> Parse(const utility::CommandLine& command_line);

}