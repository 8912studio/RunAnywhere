#pragma once

#include <memory>
#include <string>
#include "module/command.h"
#include "utility/command_line.h"

namespace ra::module {

class Module {
public:
    Module() = default;
    virtual ~Module() = default;

    Module(const Module&) = delete;
    Module& operator=(const Module&) = delete;

    virtual std::shared_ptr<Command> Interpret(const utility::CommandLine& command_line) = 0;
};

}