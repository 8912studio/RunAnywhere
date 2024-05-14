#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <zaf/base/byte_array.h>
#include "module/common/text_encoding.h"

namespace ra::mod::tool::base64 {

std::optional<std::wstring> TryToInterpretDecodedDataAsText(
    const zaf::ByteArray& decoded_data,
    TextEncoding& encoding);

std::wstring InterpretDecodedDataAsText(
    const zaf::ByteArray& decoded_data,
    TextEncoding encoding);

}