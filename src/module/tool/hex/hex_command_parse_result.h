#pragma once

#include <cstddef>
#include <cstdint>

namespace ra::module::tool::hex {

class HexCommandParseResult {
public:
    std::uint64_t position{};
    std::size_t length{ 128 };
};

}