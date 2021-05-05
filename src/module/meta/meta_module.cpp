#include "module/meta/meta_module.h"
#include "module/meta/meta_command.h"
#include "module/meta/meta_command_info.h"

namespace ra::module::meta {

std::shared_ptr<Command> MetaModule::Interpret(const std::wstring& command_text) {

    if (command_text.empty()) {
        return nullptr;
    }

    if (command_text.front() != L'!') {
        return nullptr;
    }

    auto command = command_text.substr(1);

    auto command_info = GetMetaCommandInfo(command);
    if (!command_info) {
        return nullptr;
    }

    return std::make_shared<MetaCommand>(*command_info);
}

}