#include "module/meta/meta_command.h"
#include "module/meta/meta_command_prefix.h"
#include "help/markdown/element/factory.h"

using namespace ra::help::markdown::element;

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

std::wstring MetaCommand::GetKeyword() {
    return MetaCommandPrefix + command_info_.command;
}


std::shared_ptr<help::markdown::element::Element> MetaCommand::GetHelpContent() {
    
    return MakeRoot({
        MakeHeader(HeaderDepth::_2, {
            MakeInlineCode(MetaCommandPrefix + command_info_.command),
        }),
        MakeParagraph(command_info_.description),
    });
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