#include "module/user_defined/user_defined_command.h"
#include <Windows.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/creation.h>
#include "module/user_defined/preview/user_defined_command_preview_control.h"

namespace ra::module::user_defined {
namespace {

std::wstring JoinArguments(const std::vector<std::wstring>& arguments) {

    std::wstring result;

    for (std::size_t index = 0; index < arguments.size(); ++index) {

        if (index != 0) {
            result += L' ';
        }

        result += arguments[index];
    }

    return result;
}


std::filesystem::path ModifyActivePath(
    const std::filesystem::path& path,
    const std::wstring& modifier) {

    std::size_t backward_level{};
    for (std::size_t index = 1; index < modifier.length(); ++index) {

        if (modifier[index] == L'.') {
            ++backward_level;
        }
        else {
            //Invalid char in modifier, return original path.
            return path;
        }
    }

    return GetBackwardedActivePath(path, backward_level);
}

}

UserDefinedCommand::UserDefinedCommand(
    const UserDefinedEntry& entry, 
    const std::vector<std::wstring>& input_arguments)
    :
    entry_(entry),
    input_arguments_(input_arguments) {

}


std::shared_ptr<CommandPreviewControl> UserDefinedCommand::GetPreviewControl() {

    auto control = zaf::Create<UserDefinedCommandPreviewControl>();
    control->SetParseResult(ParseCommandLine());
    return control;
}


void UserDefinedCommand::Execute() {

    auto parse_result = ParseCommandLine();

    ShellExecute(
        nullptr,
        nullptr,
        parse_result.command.c_str(),
        JoinArguments(parse_result.arguments).c_str(),
        nullptr,
        SW_SHOWNORMAL);
}


ParseResult UserDefinedCommand::ParseCommandLine() {

    std::filesystem::path modified_active_path;
    std::vector<std::wstring> plain_arguments;
    ParseArguments(modified_active_path, plain_arguments);

    return user_defined::ParseCommandLine(
        entry_.command_line,
        modified_active_path, 
        plain_arguments);
}


void UserDefinedCommand::ParseArguments(
    std::filesystem::path& modified_active_path,
    std::vector<std::wstring>& plain_arguments) {

    modified_active_path = GetDesktopContext().active_path;

    if (input_arguments_.empty()) {
        return;
    }

    const auto& first_argument = input_arguments_[0];
    if (first_argument[0] == L'@') {
        modified_active_path = ModifyActivePath(modified_active_path, first_argument);
    }
    else {
        plain_arguments.push_back(first_argument);
    }

    for (std::size_t index = 1; index < input_arguments_.size(); ++index) {
        plain_arguments.push_back(input_arguments_[index]);
    }
}

}