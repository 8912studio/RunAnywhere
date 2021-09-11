#pragma once

#include <optional>
#include <string>
#include "module/date/date_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::module::date {

constexpr const wchar_t* const DateCommandLiteral = L"date";

std::optional<DateCommandParseResult> ParseDateCommand(const utility::CommandLine& command_line);

}