#include "module/meta/meta_command.h"
#include "module/meta/meta_command_prefix.h"

namespace ra::mod::meta {
namespace {

class MetaCommandExecutor : public CommandExecutor {
public:
    explicit MetaCommandExecutor(std::function<void()> handler) : handler_(std::move(handler)) {

    }

    void Execute() {
        handler_();
    }

private:
    std::function<void()> handler_;
};

}

CommandBrief MetaCommand::GetBrief() {
    return CommandBrief{ MetaCommandPrefix + command_info_.command, command_info_.description };
}


help::content::Content MetaCommand::GetHelpContent() {

    help::content::Content result;
    result.AddTitleLine(MetaCommandPrefix + command_info_.command + L" command");
    result.AddBodyLine(command_info_.description);
    return result;
}


bool MetaCommand::Interpret(
    const utility::CommandLine& command_line,
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    //Not allow to reuse.
    if (is_reusing) {
        return false;
    }
    return true;
}


std::shared_ptr<CommandExecutor> MetaCommand::GetExecutor() {
    return std::make_shared<MetaCommandExecutor>(command_info_.handler);
}

}