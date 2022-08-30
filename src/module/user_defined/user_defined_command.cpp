#include "module/user_defined/user_defined_command.h"
#include <Windows.h>
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/container/utility/range.h>
#include <zaf/base/string/join.h>
#include <zaf/creation.h>
#include "environment_variable_manager.h"
#include "module/active_path/active_path_modifying.h"
#include "module/active_path/active_path_option_parsing.h"
#include "module/user_defined/parse/variable_formatter.h"
#include "module/user_defined/preview/user_defined_command_preview_control.h"

namespace ra::module::user_defined {
namespace {

context::ActivePath ModifyActivePath(
    const context::ActivePath& active_path,
    const std::wstring& modifier) {

    auto option = active_path::ParseActivePathOption(modifier.substr(1));
    return active_path::ModifyActivePathByOption(active_path, option);
}


std::wstring JoinArguments(const std::vector<std::wstring>& arguments) {
    return zaf::JoinAsWideString(arguments);
}

}


UserDefinedCommand::UserDefinedCommand(
    const std::shared_ptr<Entry>& entry,
    const std::vector<std::wstring>& input_arguments)
    :
    entry_(entry),
    input_arguments_(input_arguments) {

}


help::content::Content UserDefinedCommand::GetHelpContent() {

    help::content::Content result;
    result.AddTitleLine(entry_->Keyword() + L" command");
    result.AddBodyLine(L"User-defined command.");
    return result;
}


std::shared_ptr<CommandPreviewControl> UserDefinedCommand::GetPreviewControl() {

    auto control = zaf::Create<UserDefinedCommandPreviewControl>();
    control->SetParseResult(ParseCommandLine());
    return control;
}


void UserDefinedCommand::Execute() {

    auto parse_result = ParseCommandLine();
    if (parse_result.command.empty()) {
        return;
    }

    //Update current process' environment variables in order to inherit them in child process.
    EnvironmentVariableManager::Instance().Update();

    ShellExecute(
        nullptr,
        nullptr,
        parse_result.command.c_str(),
        JoinArguments(parse_result.arguments).c_str(),
        nullptr,
        SW_SHOWNORMAL);
}


EntryCommandParseResult UserDefinedCommand::ParseCommandLine() {

    context::ActivePath modified_active_path;
    std::vector<std::wstring> plain_arguments;
    ParseArguments(modified_active_path, plain_arguments);

    VariableFormatter variable_formatter{ entry_->BundleMeta(), modified_active_path };

    return user_defined::ParseEntryCommand(
        entry_->Command(),
        variable_formatter,
        plain_arguments);
}


void UserDefinedCommand::ParseArguments(
    context::ActivePath& modified_active_path,
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