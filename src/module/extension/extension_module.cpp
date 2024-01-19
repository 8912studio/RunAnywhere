#include "module/extension/extension_module.h"
#include "module/extension/command_line_bridge.h"
#include "module/extension/extension_command.h"

namespace ra::mod::extension {

ExtensionModule::ExtensionModule(const std::filesystem::path& path) : extension_path_(path) {

    interface_.module_handle = LoadLibrary(path.c_str());
    if (!interface_.module_handle) {
        return;
    }

    interface_.create_function = 
        reinterpret_cast<decltype(interface_.create_function)>(
            GetProcAddress(interface_.module_handle, "RA_Create"));

    interface_.interpret_function = 
        reinterpret_cast<decltype(interface_.interpret_function)>(
            GetProcAddress(interface_.module_handle, "RA_Interpret"));

    interface_.get_preview_text_function = 
        reinterpret_cast<decltype(interface_.get_preview_text_function)>(
            GetProcAddress(interface_.module_handle, "RA_GetPreviewText"));

    interface_.destroy_function = 
        reinterpret_cast<decltype(interface_.destroy_function)>(
            GetProcAddress(interface_.module_handle, "RA_Destroy"));
}


ExtensionModule::~ExtensionModule() {

    if (interface_.module_handle) {
        FreeLibrary(interface_.module_handle);
    }
}


std::unique_ptr<Command> ExtensionModule::CreateCommand(const utility::CommandLine& command_line) {

    if (!interface_.create_function ||
        !interface_.interpret_function ||
        !interface_.get_preview_text_function ||
        !interface_.destroy_function) {
        return nullptr;
    }

    CommandLineBridge command_line_bridge{ command_line };
    auto command_handle = interface_.create_function(command_line_bridge.PlainStruct());
    if (!command_handle) {
        return nullptr;
    }

    return std::make_unique<ExtensionCommand>(command_handle, interface_);
}

}