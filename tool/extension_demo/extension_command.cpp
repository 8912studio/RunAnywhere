#include "extension_command.h"

bool ExtensionCommand::Interpret(const RA_CommandLine* command_line) {

    text_ = L"This is an extension demo. Arguments: ";

    for (int32_t index = 0; index < command_line->argument_count; ++index) {

        if (index != 0) {
            text_ += L' ';
        }

        const auto& argument = command_line->arguments[index];
        text_ += argument.content;
    }

    return true;
}
