#include "module/rgb/rgb_module.h"
#include "module/rgb/rgb_command.h"
#include "module/rgb/rgb_command_parsing.h"

namespace ra::module::rgb {

std::vector<CommandBrief> RGBModule::QuerySuggestedCommands(const std::wstring& command_text) {

    std::wstring rgb_command{ RGBCommandLiteral };
    if (rgb_command.find(command_text) == 0) {

        return {
            CommandBrief{ rgb_command, L"Show color using RGB format" },
        };
    }

    return {};
}


std::shared_ptr<Command> RGBModule::Interpret(const utility::CommandLine& command_line) {

    auto parse_result = ParseRGBCommand(command_line);
    if (!parse_result) {
        return nullptr;
    }

    return std::make_shared<RGBCommand>(*parse_result);
}

}