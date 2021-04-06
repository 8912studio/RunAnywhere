#pragma once

#include "module/calculator/parse/parser.h"

bool TestNumberParserSuccess(
    calculator::Parser& parser,
    const std::wstring& input, 
    const std::wstring& expected_output,
    int expected_base);

bool TestNumberParserFailure(
    calculator::Parser& parser,
    const std::wstring& input,
    calculator::ParseStatus expected_status);