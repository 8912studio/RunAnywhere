#pragma once

#include <filesystem>
#include "module/extension/extension_interface.h"
#include "module/module.h"

namespace ra::module::extension {

class ExtensionModule : public Module {
public:
    ExtensionModule(const std::filesystem::path& path);
    ~ExtensionModule();

    std::shared_ptr<Command> Interpret(const utility::CommandLine& command_line) override;

private:
    std::filesystem::path extension_path_;
    ExtensionInterface interface_;
};

}