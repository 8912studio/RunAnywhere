#pragma once

#include "module/command.h"
#include "module/meta/meta_command_info.h"

namespace ra::module::meta {

class MetaCommand : public Command {
public:
    explicit MetaCommand(const MetaCommandInfo& command_info) : command_info_(command_info) { 
    
    }

    std::wstring GetPreviewText() override {
        return command_info_.description;
    }

    void Execute() override {
        command_info_.handler();
    }

private:
    MetaCommandInfo command_info_;
};

}