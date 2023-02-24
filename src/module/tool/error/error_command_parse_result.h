#pragma once

#include <cstdint>

namespace ra::module::tool::error {

class ErrorCommandParseResult {
public:
    std::uint32_t error_code{};
};

}