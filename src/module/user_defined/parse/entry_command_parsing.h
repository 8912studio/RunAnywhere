#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "context/active_path.h"
#include "module/user_defined/command_line_info.h"
#include "module/user_defined/parse/variable_formatter.h"

namespace ra::mod::user_defined {

CommandLineInfo ParseEntryCommand(
    const std::wstring& entry_command,
    const VariableFormatter& variable_formatter,
    const std::vector<std::wstring>& input_arguments);

}