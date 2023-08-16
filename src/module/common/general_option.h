#pragma once

#include <optional>
#include <string>
#include "module/active_path/active_path_option.h"
#include "module/common/text_encoding.h"
#include "utility/command_line.h"

namespace ra::mod {

class GeneralOption {
public:
    std::optional<active_path::ActivePathOption> active_path_option;
    std::wstring text;
    bool treat_text_as_file{};
    std::optional<TextEncoding> text_encoding;
};


bool ParseGeneralOption(const utility::CommandLinePiece& argument, GeneralOption& option);

}