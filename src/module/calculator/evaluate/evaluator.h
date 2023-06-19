#pragma once

#include <memory>
#include "module/calculator/evaluate/evaluate_status.h"
#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/syntax_node.h"

namespace ra::mod::calculator {

class Evaluator {
public:
    static std::shared_ptr<Evaluator> Create(const std::shared_ptr<SyntaxNode>& syntax_node);

public:
    Evaluator() = default;
    virtual ~Evaluator() = default;

    Evaluator(const Evaluator&) = delete;
    Evaluator& operator=(const Evaluator&) = delete;

    virtual EvaluateStatus Evaluate(EvaluateResult& result) = 0;
};

}