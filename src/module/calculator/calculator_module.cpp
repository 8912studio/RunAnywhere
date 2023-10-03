#include "module/calculator/calculator_module.h"
#include "module/calculator/calculator_command.h"
#include "module/calculator/command_parsing.h"
#include "module/calculator/evaluate/evaluator.h"

namespace ra::mod::calculator {

std::vector<CommandBrief> CalculatorModule::QuerySuggestedCommands(
    const std::wstring& command_text) {

    if (command_text.empty()) {
        return { 
            CommandBrief{ L"", L"Input mathematical expression to invoke the calculator." }
        };
    }

    return {};
}


std::unique_ptr<Command> CalculatorModule::CreateCommand(const utility::CommandLine& command_line) {

    auto parse_result = ParseCommand(command_line.RawText());
    if (!parse_result) {
        return nullptr;
    }

    auto evaluator = Evaluator::Create(parse_result->GetExpressionRootNode());

    EvaluateResult evaluate_result;
    auto evaluate_status = evaluator->Evaluate(evaluate_result);
    if (evaluate_status != EvaluateStatus::Ok) {
        return nullptr;
    }

    return std::make_unique<CalculatorCommand>(evaluate_result, parse_result->GetModifier());
}

}