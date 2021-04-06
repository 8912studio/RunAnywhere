#pragma once

#include <memory>
#include <string>
#include "module/command.h"

class Module {
public:
    Module() = default;
    virtual ~Module() = default;

    Module(const Module&) = delete;
    Module& operator=(const Module&) = delete;

    virtual std::shared_ptr<Command> Interpret(const std::wstring& command_text) = 0;
};