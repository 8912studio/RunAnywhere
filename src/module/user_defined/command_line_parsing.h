#pragma once

#include <filesystem>
#include <string>
#include <vector>

namespace ra::module::user_defined {

class ParseResult {
public:
    std::wstring command;
    std::vector<std::wstring> arguments;
};

ParseResult ParseCommandLine(
    const std::wstring& command_line,
    const std::filesystem::path& active_path,
    const std::vector<std::wstring>& input_arguments);

std::wstring GetBackwardedActivePath(
    const std::filesystem::path& active_path,
    std::size_t backward_level);

}