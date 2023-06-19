#pragma once

#include "module/calculator/parse/parser.h"

bool TestNumberParserSuccess(
    ra::mod::calculator::Parser& parser,
    const std::wstring& input, 
    const std::wstring& expected_output,
    int expected_base);

bool TestNumberParserFailure(
    ra::mod::calculator::Parser& parser,
    const std::wstring& input,
    ra::mod::calculator::ParseStatus expected_status,
    std::size_t expected_parsed_length);