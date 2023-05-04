#include "module/extension/extension_module.h"
#include "module/extension/extension_command.h"

namespace ra::module::extension {

ExtensionModule::ExtensionModule(const std::filesystem::path& path) : extension_path_(path) {

    interface_.module_handle = LoadLibrary(path.c_str());
    if (!interface_.module_handle) {
        return;
    }

    interface_.interpret_function = 
        reinterpret_cast<decltype(interface_.interpret_function)>(
            GetProcAddress(interface_.module_handle, "RunAnywhere_Interpret"));

    interface_.get_text_function = 
        reinterpret_cast<decltype(interface_.get_text_function)>(
            GetProcAddress(interface_.module_handle, "RunAnywhere_GetText"));

    interface_.destroy_function = reinterpret_cast<decltype(interface_.destroy_function)>(
        GetProcAddress(interface_.module_handle, "RunAnywhere_Destroy"));
}


ExtensionModule::~ExtensionModule() {

    if (interface_.module_handle) {
        FreeLibrary(interface_.module_handle);
    }
}


std::shared_ptr<Command> ExtensionModule::Interpret(const utility::CommandLine& command_line) {

    if (!interface_.interpret_function ||
        !interface_.get_text_function ||
        !interface_.destroy_function) {
        return nullptr;
    }

    auto command_handle = interface_.interpret_function(command_line.RawText().c_str());
    if (!command_handle) {
        return nullptr;
    }

    return std::make_shared<ExtensionCommand>(command_handle, interface_);
}

}