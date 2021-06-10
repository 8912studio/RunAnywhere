#pragma once

#include <optional>
#include "module/crypto/md5/md5_command_parse_result.h"

namespace ra::module::crypto{

std::optional<MD5CommandParseResult> ParseMD5Command(const std::wstring& command);

}