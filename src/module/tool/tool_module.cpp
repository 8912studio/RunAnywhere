#include "module/tool/tool_module.h"
#include <functional>
#include "module/command_brief.h"
#include "module/tool/date/date_command.h"
#include "module/tool/hex/hex_command.h"
#include "module/tool/md5/md5_command.h"
#include "module/tool/rgb/rgb_command.h"

namespace ra::module::tool {

using CommandFactory = std::function<std::shared_ptr<Command>(const utility::CommandLine&)>;

class ToolCommandInfo {
public:
    ToolCommandInfo(const CommandBrief& brief, const CommandFactory& factory) :
        brief_(brief),
        factory_(factory) {

    }

    const CommandBrief& Brief() const {
        return brief_;
    }

    const CommandFactory& Fatory() const {
        return factory_;
    }

private:
    CommandBrief brief_;
    CommandFactory factory_;
};


template<typename T>
std::unique_ptr<ToolCommandInfo> CreateCommandInfo() {

    return std::make_unique<ToolCommandInfo>(
        T::GetBrief(),
        [](const utility::CommandLine& command_line) {
            return std::make_shared<T>(command_line);
        }
    );
}


ToolModule::ToolModule() {

    command_infos_.push_back(CreateCommandInfo<date::DateCommand>());
    command_infos_.push_back(CreateCommandInfo<hex::HexCommand>());
    command_infos_.push_back(CreateCommandInfo<md5::MD5Command>());
    command_infos_.push_back(CreateCommandInfo<rgb::RGBCommand>());
}


ToolModule::~ToolModule() {

}


std::vector<CommandBrief> ToolModule::QuerySuggestedCommands(const std::wstring& command_text) {

    std::vector<CommandBrief> result;

    for (const auto& each_info : command_infos_) {

        const auto& command_brief = each_info->Brief();
        if (command_brief.Command().find(command_text) == 0) {
            result.push_back(command_brief);
        }
    }

    return result;
}


std::shared_ptr<Command> ToolModule::Interpret(const utility::CommandLine& command_line) {

    for (const auto& each_info : command_infos_) {

        if (each_info->Brief().Command() == command_line.Command()) {
            return each_info->Fatory()(command_line);
        }
    }

    return nullptr;
}

}