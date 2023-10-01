#include "module/tool/tool_module.h"
#include <functional>
#include "help/built_in_help_content_manager.h"
#include "module/command_brief.h"
#include "module/tool/base64/base64_command.h"
#include "module/tool/date/date_command.h"
#include "module/tool/error/error_command.h"
#include "module/tool/hash/md5_command.h"
#include "module/tool/hash/sha_command.h"
#include "module/tool/hex/hex_command.h"
#include "module/tool/rgb/rgb_command.h"
#include "module/tool/text_transform/lower_transformer.h"
#include "module/tool/text_transform/text_transform_command.h"
#include "module/tool/text_transform/upper_transformer.h"

namespace ra::mod::tool {

using CommandFactory = std::function<std::unique_ptr<Command>()>;

class ToolCommandInfo {
public:
    ToolCommandInfo(std::wstring keyword, CommandFactory factory) :
        keyword_(std::move(keyword)),
        factory_(std::move(factory)) {

    }

    const std::wstring& Keyword() const {
        return keyword_;
    }

    const CommandFactory& Factory() const {
        return factory_;
    }

private:
    std::wstring keyword_;
    CommandFactory factory_;
};


template<typename T>
std::unique_ptr<ToolCommandInfo> CreateCommandInfo() {
    return std::make_unique<ToolCommandInfo>(T::Keyword(), []() {
        return std::make_unique<T>();
    });
}


template<typename T>
std::unique_ptr<ToolCommandInfo> CreateTextTransformCommandInfo() {

    return std::make_unique<ToolCommandInfo>(T::Keyword(), []() {
        return std::make_unique<text_transform::TextTransformCommand>(std::make_unique<T>());
    });
}


ToolModule::ToolModule() {

    command_infos_.push_back(CreateCommandInfo<base64::Base64Command>());
    command_infos_.push_back(CreateCommandInfo<date::DateCommand>());
    command_infos_.push_back(CreateCommandInfo<error::ErrorCommand>());
    command_infos_.push_back(CreateCommandInfo<hash::MD5Command>());
    command_infos_.push_back(CreateCommandInfo<hash::SHACommand>());
    command_infos_.push_back(CreateCommandInfo<hex::HexCommand>());
    command_infos_.push_back(CreateCommandInfo<rgb::RGBCommand>());
    command_infos_.push_back(CreateTextTransformCommandInfo<text_transform::LowerTransformer>());
    command_infos_.push_back(CreateTextTransformCommandInfo<text_transform::UpperTransformer>());
}


ToolModule::~ToolModule() {

}


std::vector<CommandBrief> ToolModule::QuerySuggestedCommands(const std::wstring& command_text) {

    std::vector<CommandBrief> result;

    for (const auto& each_info : command_infos_) {

        const auto& keyword = each_info->Keyword();
        if (keyword.find(command_text) == 0) {

            auto description = help::BuiltInHelpContentManager::Instance().GetDescription(keyword);
            ZAF_EXPECT(description);

            result.emplace_back(keyword, description);
        }
    }

    return result;
}


std::unique_ptr<Command> ToolModule::CreateCommand(const utility::CommandLine& command_line) {

    for (const auto& each_info : command_infos_) {

        if (each_info->Keyword() == command_line.Command()) {
            return each_info->Factory()();
        }
    }

    return nullptr;
}

}