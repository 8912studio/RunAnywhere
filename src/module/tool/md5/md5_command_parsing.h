#pragma once

#include "module/tool/md5/md5_command_parse_result.h"
#include "utility/command_line.h"

namespace ra::mod::tool::md5 {

MD5CommandParseResult ParseMD5Command(const utility::CommandLine& command_line);

}