#pragma once

#include <cstddef>
#include <string>
#include <vector>
#include "module/common/text_encoding.h"

namespace ra::mod::tool::base64 {

std::wstring TryToInterpretDecodedDataAsText(
    const std::vector<std::byte>& decoded_data,
    TextEncoding& encoding);;

}