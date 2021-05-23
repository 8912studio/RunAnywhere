#pragma once

#include "module/calculator/parse/parser.h"

bool TestNumberParserSuccess(
    ra::module::calculator::Parser& parser,
    const std::wstring& input, 
    const std::wstring& expected_output,
    int expected_base);

bool TestNumberParserFailure(
    ra::module::calculator::Parser& parser,
    const std::wstring& input,
    ra::module::calculator::ParseStatus expected_status,
    std::size_t expected_parsed_length);