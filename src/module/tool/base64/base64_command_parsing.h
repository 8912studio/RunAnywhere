#pragma once

#include <optional>
#include <string>
#include "utility/command_line.h"

namespace ra::mod::tool::base64 {

enum class Base64Operation {
    Encode,
    Decode,
};

class Base64CommandParseResult {
public:
    std::wstring input_text;
    std::optional<Base64Operation> operation;
};


Base64CommandParseResult Parse(const utility::CommandLine& command_line);

}