#pragma once

#include "module/command.h"
#include "module/meta/meta_command_info.h"

namespace ra::mod::meta {

class MetaCommand : public Command {
public:
    explicit MetaCommand(const MetaCommandInfo& command_info) : command_info_(command_info) { 
    
    }

    CommandBrief GetBrief() override;
    std::shared_ptr<help::markdown::element::Element> GetHelpContent() override;

    bool Interpret(
        const utility::CommandLine& command_line,
        const context::DesktopContext& desktop_context, 
        bool is_reusing) override;

    std::wstring GetPreviewText() override {
        return command_info_.description;
    }

    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    MetaCommandInfo command_info_;
};

}