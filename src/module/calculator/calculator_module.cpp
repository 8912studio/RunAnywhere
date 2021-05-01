#include "module/calculator/calculator_module.h"
#include "module/calculator/calculator_command.h"
#include "module/calculator/parse/command_parser.h"
#include "module/calculator/evaluate/evaluator.h"

namespace ra::module::calculator {

std::shared_ptr<Command> CalculatorModule::Interpret(const std::wstring& command_text) {

    ParseContext parse_context{ command_text };
    ParseResult parse_result;

    auto parse_status = CommandParser::Instance()->Parse(parse_context, parse_result);
    if (parse_status != ParseStatus::Ok) {
        return nullptr;
    }

    auto evaluator = Evaluator::Create(parse_result.GetExpressionRootNode());

    EvaluateResult evaluate_result;
    auto evaluate_status = evaluator->Evaluate(evaluate_result);
    if (evaluate_status != EvaluateStatus::Ok) {
        return nullptr;
    }

    return std::make_shared<CalculatorCommand>(evaluate_result, parse_result.GetModifier());
}

}