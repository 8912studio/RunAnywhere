#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <vector>
#include "module/common/text_encoding.h"

namespace ra::mod::tool::base64 {

std::optional<std::wstring> TryToInterpretDecodedDataAsText(
    const std::vector<std::byte>& decoded_data,
    TextEncoding& encoding);

std::wstring InterpretDecodedDataAsText(
    const std::vector<std::byte>& decoded_data, 
    TextEncoding encoding);

}