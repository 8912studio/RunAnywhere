#include "module/calculator/evaluate/operand_evaluator.h"
#include <cctype>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/to_numeric.h>
#include "module/calculator/evaluate/evaluate_result.h"
#include "module/calculator/parse/operand_node.h"

namespace ra::mod::calculator {

EvaluateStatus OperandEvaluator::Evaluate(EvaluateResult& result) {

    if (operand_node_->base == 10) {

        result.decimal_value.assign(zaf::ToUTF8String(operand_node_->text));

        switch (operand_node_->unit) {
        case NumberUnit::Tera:
            result.decimal_value *= 1024;
        case NumberUnit::Giga:
            result.decimal_value *= 1024;
        case NumberUnit::Mega:
            result.decimal_value *= 1024;
        case NumberUnit::Kilo:
            result.decimal_value *= 1024;
        default:
            break;
        }
    }
    else {

        std::int64_t integer_value = 0;
        bool is_succeeded = zaf::TryToNumeric(
            operand_node_->text,
            integer_value,
            zaf::ToNumericOptions().Base(static_cast<int>(operand_node_->base)));

        if (!is_succeeded) {
            return EvaluateStatus::Overflow;
        }

        result.decimal_value = integer_value;
    }

    return EvaluateStatus::Ok;
}

}