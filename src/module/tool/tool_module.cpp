#include "module/tool/tool_module.h"
#include "module/tool/date/date_command_factory.h"
#include "module/tool/md5/md5_command_factory.h"
#include "module/tool/rgb/rgb_command_factory.h"

namespace ra::module::tool {

ToolModule::ToolModule() {

    command_factories_.push_back(std::make_unique<date::DateCommandFactory>());
    command_factories_.push_back(std::make_unique<md5::MD5CommandFactory>());
    command_factories_.push_back(std::make_unique<rgb::RGBCommandFactory>());
}


std::vector<CommandBrief> ToolModule::QuerySuggestedCommands(const std::wstring& command_text) {

    std::vector<CommandBrief> result;

    for (const auto& each_factory : command_factories_) {

        const auto& command_brief = each_factory->CommandBrief();
        if (command_brief.Command().find(command_text) == 0) {
            result.push_back(command_brief);
        }
    }

    return result;
}


std::shared_ptr<Command> ToolModule::Interpret(const utility::CommandLine& command_line) {

    for (const auto& each_factory : command_factories_) {

        if (each_factory->CommandBrief().Command() == command_line.Command()) {
            return each_factory->CreateCommand(command_line);
        }
    }

    return nullptr;
}

}