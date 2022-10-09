#pragma once

#include <cstddef>
#include <cstdint>

namespace ra::module::tool::hex {

class HexCommandParseResult {
public:
    static constexpr std::uint64_t DefaultLength = 128;

public:
    std::uint64_t position{};
    std::uint64_t length{ DefaultLength };
};

}