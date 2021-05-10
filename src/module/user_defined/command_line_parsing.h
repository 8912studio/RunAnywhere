#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "context/active_path.h"

namespace ra::module::user_defined {

class ParseResult {
public:
    std::wstring command;
    std::vector<std::wstring> arguments;
};

ParseResult ParseCommandLine(
    const std::wstring& command_line,
    const context::ActivePath& active_path,
    const std::vector<std::wstring>& input_arguments);
}