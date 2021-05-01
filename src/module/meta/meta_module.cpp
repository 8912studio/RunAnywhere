#include "module/meta/meta_module.h"
#include "module/meta/meta_command.h"

namespace ra::module::meta {

std::shared_ptr<Command> MetaModule::Interpret(const std::wstring& command_text) {

    if (command_text.empty()) {
        return nullptr;
    }

    if (command_text.front() != L'!') {
        return nullptr;
    }

    return std::make_shared<MetaCommand>(command_text.substr(1));
}

}