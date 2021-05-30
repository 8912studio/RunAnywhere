#pragma once

#include <optional>
#include <string>
#include "module/date/date_command_parse_result.h"

namespace ra::module::date {

std::optional<DateCommandParseResult> ParseDateCommand(const std::wstring& command);

}