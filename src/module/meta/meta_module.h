#pragma once

#include "module/module.h"

class MetaModule : public Module {
public:
    std::shared_ptr<Command> Interpret(const std::wstring& command_text) override;
};