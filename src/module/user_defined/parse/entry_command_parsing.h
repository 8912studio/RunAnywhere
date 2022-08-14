#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include "context/active_path.h"

namespace ra::module::user_defined {

class EntryCommandParseResult {
public:
    std::wstring command;
    std::vector<std::wstring> arguments;
};

EntryCommandParseResult ParseEntryCommand(
    const std::wstring& entry_command,
    const context::ActivePath& active_path,
    const std::vector<std::wstring>& input_arguments);

}