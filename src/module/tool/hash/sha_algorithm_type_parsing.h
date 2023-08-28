#pragma once

#include <optional>
#include "module/tool/hash/sha_algorithm_type.h"
#include "utility/command_line.h"

namespace ra::mod::tool::hash {

std::optional<SHAAlgorithmType> ParseSHAAlgorithmType(const utility::CommandLine& command_line);

}