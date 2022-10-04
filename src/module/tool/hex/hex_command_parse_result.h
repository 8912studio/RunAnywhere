#pragma once

#include <cstddef>
#include <cstdint>

namespace ra::module::tool::hex {

class HexCommandParseResult {
public:
    static constexpr std::size_t DefaultLength = 128;

public:
    std::uint64_t position{};
    std::size_t length{ DefaultLength };
};

}