#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "context/active_path.h"
#include "module/user_defined/parse/variable_formatter.h"

namespace ra::module::user_defined {

class EntryCommandParseResult {
public:
    std::wstring command;
    std::vector<std::wstring> arguments;
};

EntryCommandParseResult ParseEntryCommand(
    const std::wstring& entry_command,
    const VariableFormatter& variable_formatter,
    const std::vector<std::wstring>& input_arguments);

}