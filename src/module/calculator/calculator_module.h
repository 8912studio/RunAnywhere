#pragma once

#include "module/module.h"

namespace ra::module::calculator {

class CalculatorModule : public Module {
public:
    std::shared_ptr<Command> Interpret(const utility::CommandLine& command_line) override;
};

}