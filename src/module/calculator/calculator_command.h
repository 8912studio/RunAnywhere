#pragma once

#include <Windows.h>
#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/modifier.h"
#include "module/command.h"

namespace ra::mod::calculator {

class CalculatorCommand : public Command {
public:
    static CommandBrief Brief();

public:
    CalculatorCommand(
        const calculator::EvaluateResult& evaluate_result, 
        const calculator::Modifier& modifier);

    CommandBrief GetBrief() override;
    bool Interpret(
        const utility::CommandLine& command_line, 
        const context::DesktopContext& desktop_context, 
        bool is_reusing) override;
    std::shared_ptr<CommandPreviewControl> GetPreviewControl() override;
    std::shared_ptr<CommandExecutor> GetExecutor() override;

private:
    calculator::EvaluateResult evaluate_result_;
    calculator::Modifier modifier_;
};

}