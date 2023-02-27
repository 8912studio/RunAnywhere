#pragma once

#include <optional>
#include <string>
#include "module/calculator/evaluate/evaluate_result.h"

namespace ra::utility {

enum class NumberParseStatus {

    OK,

    //The number text is incomplete, such as `x` or `0x`.
    //The parsing should not treat as error in such cases.
    Incomplete,

    //The text is not a number.
    NotNumber,
};


//Parse decimal or hex number from text.
NumberParseStatus ParseNumber(
    const std::wstring& text,
    module::calculator::EvaluateResult& result);

}