#include "module/tool/tool_module.h"
#include <functional>
#include "module/command_brief.h"
#include "module/tool/base64/base64_command.h"
#include "module/tool/date/date_command.h"
#include "module/tool/error/error_command.h"
#include "module/tool/hex/hex_command.h"
#include "module/tool/md5/md5_command.h"
#include "module/tool/rgb/rgb_command.h"
#include "module/tool/text_transform/lower_transformer.h"
#include "module/tool/text_transform/text_transform_command.h"
#include "module/tool/text_transform/upper_transformer.h"

namespace ra::mod::tool {

using CommandFactory = std::function<std::unique_ptr<Command>()>;

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
    return std::make_unique<ToolCommandInfo>(T::Brief(), []() {
        return std::make_unique<T>();
    });
}


template<typename T>
std::unique_ptr<ToolCommandInfo> CreateTextTransformCommandInfo() {

    return std::make_unique<ToolCommandInfo>(T::Brief(), []() {
        return std::make_unique<text_transform::TextTransformCommand>(std::make_unique<T>());
    });
}


ToolModule::ToolModule() {

    command_infos_.push_back(CreateCommandInfo<base64::Base64Command>());
    command_infos_.push_back(CreateCommandInfo<date::DateCommand>());
    command_infos_.push_back(CreateCommandInfo<error::ErrorCommand>());
    command_infos_.push_back(CreateCommandInfo<hex::HexCommand>());
    command_infos_.push_back(CreateCommandInfo<md5::MD5Command>());
    command_infos_.push_back(CreateCommandInfo<rgb::RGBCommand>());
    command_infos_.push_back(CreateTextTransformCommandInfo<text_transform::LowerTransformer>());
    command_infos_.push_back(CreateTextTransformCommandInfo<text_transform::UpperTransformer>());
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


std::unique_ptr<Command> ToolModule::CreateCommand(const utility::CommandLine& command_line) {

    for (const auto& each_info : command_infos_) {

        if (each_info->Brief().Command() == command_line.Command()) {
            return each_info->Fatory()();
        }
    }

    return nullptr;
}

}