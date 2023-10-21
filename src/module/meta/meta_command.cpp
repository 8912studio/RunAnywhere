#include "module/meta/meta_command.h"
#include "module/meta/meta_command_prefix.h"
#include "utility/markdown/element/factory.h"

using namespace ra::utility::markdown::element;

namespace ra::mod::meta {
namespace {

class MetaCommandExecutor : public CommandExecutor {
public:
    explicit MetaCommandExecutor(std::function<void()> handler) : handler_(std::move(handler)) {

    }

    ExecuteResult Execute() {
        handler_();
        return PostExecuteAction::Dispose;
    }

private:
    std::function<void()> handler_;
};

}

std::wstring MetaCommand::GetKeyword() {
    return MetaCommandPrefix + command_info_.command;
}


help::HelpContent MetaCommand::GetHelpContent() {

    auto element = MakeRoot({
        MakeHeader(HeaderDepth::_2, {
            MakeInlineCode(GetKeyword()),
        }),
        MakeParagraph(command_info_.description),
    });

    return { GetKeyword(), element };
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