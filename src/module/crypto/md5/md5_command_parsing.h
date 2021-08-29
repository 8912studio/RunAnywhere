#pragma once

#include <optional>
#include "module/crypto/md5/md5_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::module::crypto{

std::optional<MD5CommandParseResult> ParseMD5Command(const utility::CommandLine& command_line);

}