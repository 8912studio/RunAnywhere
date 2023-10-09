#include "module/chat_gpt/chat_gpt_command.h"
#include "help/markdown/element/factory.h"
#include "module/chat_gpt/chat_gpt_executor.h"

namespace ra::mod::chat_gpt {

ChatGPTCommand::ChatGPTCommand(std::shared_ptr<comm::OpenAIClient> client) : 
    client_(std::move(client)) {

}


std::wstring ChatGPTCommand::GetKeyword() {
    return std::wstring();
}


help::HelpContent ChatGPTCommand::GetHelpContent() {
    return help::HelpContent{
        L"",
        help::markdown::element::MakeRoot({})
    };
}


bool ChatGPTCommand::Interpret(
    const utility::CommandLine& command_line, 
    const context::DesktopContext& desktop_context,
    bool is_reusing) {

    return true;
}


std::wstring ChatGPTCommand::GetPreviewText() {
    return L"test";
}


std::shared_ptr<CommandPreviewControl> ChatGPTCommand::GetPreviewControl() {
    return nullptr;
}


std::shared_ptr<CommandExecutor> ChatGPTCommand::GetExecutor() {
    return std::make_shared<ChatGPTExecutor>(client_);
}

}