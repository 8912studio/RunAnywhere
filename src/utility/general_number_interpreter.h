#pragma once

#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/parser.h"

namespace ra::utility {

class GeneralNumberInterpreter {
public:
    enum class Status {
        OK,

        //The number text is incomplete, such as `x` or `0x`.
        //The parsing should not treat as error in such cases.
        Incomplete,

        //The text is not a number.
        NotNumber,
    };

public:
    explicit GeneralNumberInterpreter(bool supports_negative_decimal);

    Status Interpret(const std::wstring& input, module::calculator::EvaluateResult& result);

private:
    Status Parse(
        const std::wstring& input,
        std::shared_ptr<module::calculator::SyntaxNode>& syntax_node);

private:
    module::calculator::Parser* parser_{};
};

}