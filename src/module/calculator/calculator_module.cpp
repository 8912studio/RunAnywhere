#include "module/calculator/calculator_module.h"
#include "module/calculator/calculator_command.h"
#include "module/calculator/command_parsing.h"
#include "module/calculator/evaluate/evaluator.h"

namespace ra::module::calculator {

std::shared_ptr<Command> CalculatorModule::Interpret(const std::wstring& command_text) {

    auto parse_result = ParseCommand(command_text);
    if (!parse_result) {
        return nullptr;
    }

    auto evaluator = Evaluator::Create(parse_result->GetExpressionRootNode());

    EvaluateResult evaluate_result;
    auto evaluate_status = evaluator->Evaluate(evaluate_result);
    if (evaluate_status != EvaluateStatus::Ok) {
        return nullptr;
    }

    return std::make_shared<CalculatorCommand>(evaluate_result, parse_result->GetModifier());
}

}