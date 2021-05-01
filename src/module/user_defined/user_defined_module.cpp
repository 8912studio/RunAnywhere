#include "module/user_defined/user_defined_module.h"
#include <Windows.h>
#include "module/user_defined/user_defined_command.h"
#include "module/user_defined/user_defined_entry_reading.h"

namespace ra::module::user_defined {

void UserDefinedModule::Reload() {

    entries_ = ReadUserDefinedEntries();
}


std::shared_ptr<Command> UserDefinedModule::Interpret(const std::wstring& command_text) {

    int argument_count{};
    auto arguments = CommandLineToArgvW(command_text.c_str(), &argument_count);
    if (!arguments) {
        return nullptr;
    }

    std::shared_ptr<Command> command;
    if (argument_count > 0) {

        auto entry = FindEntry(arguments[0]);
        if (entry) {

            std::vector<std::wstring> command_arguments;
            for (int index = 1; index < argument_count; ++index) {
                command_arguments.push_back(arguments[index]);
            }

            command = std::make_shared<UserDefinedCommand>(*entry, command_arguments);
        }
    }

    LocalFree(arguments);
    return command;
}


std::optional<UserDefinedEntry> UserDefinedModule::FindEntry(std::wstring_view keyword) {

    for (const auto& each_entry : entries_) {

        if (each_entry.keyword == keyword) {
            return each_entry;
        }
    }

    return std::nullopt;
}

}