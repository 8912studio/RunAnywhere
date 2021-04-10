#include "module/user_defined/command_line_parsing.h"
#include <Windows.h>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/string/replace.h>

namespace {

std::wstring GetFocusedPath(const std::filesystem::path& focused_path) {

    if (focused_path.empty()) {
        return {};
    }

    std::wstring focused_path_string = focused_path.wstring();
    if (!zaf::Contain(focused_path_string, L' ')) {
        return focused_path_string;
    }

    return L'"' + focused_path_string + L'"';
}


std::wstring ReplaceParameter(
    std::wstring_view argument, 
    const std::filesystem::path& focused_path,
    const std::vector<std::wstring>& input_arguments) {

    std::wstring result{ argument.data(), argument.length() };
    zaf::Replace(result, L"%@", GetFocusedPath(focused_path));
    return result;
}


std::vector<std::wstring> ReplaceParameters(
    wchar_t** parameters,
    int parameter_count,
    const std::filesystem::path& focused_path,
    const std::vector<std::wstring>& input_arguments) {

    std::vector<std::wstring> result;

    for (int index = 1; index < parameter_count; ++index) {

        auto replaced_argument = ReplaceParameter(parameters[index], focused_path, input_arguments);
        if (!replaced_argument.empty()) {

            result.push_back(replaced_argument);
        }
    }

    return result;
}


}

ParseResult ParseCommandLine(
    const std::wstring& command_line,
    const std::filesystem::path& focused_path,
    const std::vector<std::wstring>& input_arguments) {

    ParseResult result;

    int parameter_count{};
    auto parameters = CommandLineToArgvW(command_line.c_str(), &parameter_count);
    if (!parameters) {
        return result;
    }

    if (parameter_count > 0) {

        result.command = parameters[0];
        result.arguments = ReplaceParameters(
            parameters, 
            parameter_count, 
            focused_path, 
            input_arguments);
    }

    LocalFree(parameters);
    return result;
}