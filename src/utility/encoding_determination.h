#pragma once

#include <cstddef>
#include <vector>

namespace ra::utility {

bool IsUTF8Encoded(const std::vector<std::byte>& data);
bool IsUTF16Encoded(const std::vector<std::byte>& data);

}