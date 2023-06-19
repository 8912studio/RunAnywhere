#pragma once

#include <cstddef>
#include <cstdint>

namespace ra::mod::tool::hex {

class HexCommandParseResult {
public:
    static constexpr std::uint64_t DefaultLength = 128;
    static constexpr std::uint64_t MaxLength = 4096;

public:
    std::uint64_t position{};
    std::uint64_t length{ DefaultLength };
};

}