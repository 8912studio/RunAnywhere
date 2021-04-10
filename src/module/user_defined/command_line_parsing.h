#pragma once

#include <filesystem>
#include <string>
#include <vector>

class ParseResult {
public:
    std::wstring command;
    std::vector<std::wstring> arguments;
};

ParseResult ParseCommandLine(
    const std::wstring& command_line, 
    const std::filesystem::path& focused_path,
    const std::vector<std::wstring>& input_arguments);