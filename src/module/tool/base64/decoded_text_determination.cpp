#include "module/tool/base64/decoded_text_determination.h"
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/error/error.h>
#include <zaf/base/string/encoding_conversion.h>

namespace ra::mod::tool::base64 {
namespace {

std::wstring TryToInterpretDecodedDataAsUTF8String(
    const std::vector<std::byte>& decoded_data) {

    try {

        std::string_view utf8_string{
            reinterpret_cast<const char*>(decoded_data.data()),
            decoded_data.size()
        };

        return zaf::FromUTF8String(
            utf8_string,
            zaf::FromUTF8Flags::FailOnInvalidChars);
    }
    catch (const zaf::Error&) {
        return {};
    }
}

}

std::wstring TryToInterpretDecodedDataAsText(
    const std::vector<std::byte>& decoded_data,
    TextEncoding& encoding) {

    auto result = TryToInterpretDecodedDataAsUTF8String(decoded_data);
    if (!result.empty()) {

        //If there are \0 chars after conversion, we consider it is not an UTF8-8 text.
        if (!zaf::Contain(result, L'\0')) {
            encoding = TextEncoding::UTF8;
            return result;
        }
    }

    return {};
}

}