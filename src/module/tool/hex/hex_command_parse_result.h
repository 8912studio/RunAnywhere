#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include "module/active_path/active_path_option.h"

namespace ra::mod::tool::hex {

class HexCommandParseResult {
public:
    static constexpr std::uint64_t DefaultLength = 128;
    static constexpr std::uint64_t MaxLength = 4096;

public:
    std::optional<active_path::ActivePathOption> active_path_option;
    std::uint64_t position{};
    std::uint64_t length{ DefaultLength };
};

}