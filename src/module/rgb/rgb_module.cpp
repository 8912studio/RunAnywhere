#include "module/rgb/rgb_module.h"
#include "module/rgb/rgb_command.h"
#include "module/rgb/rgb_command_parsing.h"

namespace ra::module::rgb {

std::shared_ptr<Command> RGBModule::Interpret(const std::wstring& command_text) {

    auto parse_result = ParseRGBCommand(command_text);
    if (!parse_result) {
        return nullptr;
    }

    return std::make_shared<RGBCommand>(*parse_result);
}

}