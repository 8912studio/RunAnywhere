#include "module/tool/base64/decoded_data_interpreting.h"
#include <zaf/base/container/utility/contain.h>
#include <zaf/base/error/error.h>
#include <zaf/base/string/encoding_conversion.h>

namespace ra::mod::tool::base64 {
namespace {

std::optional<std::wstring> InterpretDecodedDataAsUTF8String(
    const std::vector<std::byte>& decoded_data,
    bool fail_on_invalid_char) {

    try {

        std::string_view utf8_string{
            reinterpret_cast<const char*>(decoded_data.data()),
            decoded_data.size()
        };

        auto flags = fail_on_invalid_char ? 
            zaf::FromUTF8Flags::FailOnInvalidChars : 
            zaf::FromUTF8Flags::None;

        return zaf::FromUTF8String(utf8_string, flags);
    }
    catch (const zaf::Error&) {
        return std::nullopt;
    }
}


std::wstring InterpretDecodedDataAsUTF16String(const std::vector<std::byte>& decoded_data) {

    auto utf8_string = zaf::ToUTF8String(std::wstring_view{
        reinterpret_cast<const wchar_t*>(decoded_data.data()),
        decoded_data.size()
    });

    return zaf::FromUTF8String(utf8_string);
}

}

std::optional<std::wstring> TryToInterpretDecodedDataAsText(
    const std::vector<std::byte>& decoded_data,
    TextEncoding& encoding) {

    auto result = InterpretDecodedDataAsUTF8String(decoded_data, true);
    if (!result) {
        return std::nullopt;
    }

    //If there are \0 chars after conversion, we consider it is not an UTF-8 text.
    if (zaf::Contain(*result, L'\0')) {
        return std::nullopt;
    }

    encoding = TextEncoding::UTF8;
    return result;
}


std::wstring InterpretDecodedDataAsText(
    const std::vector<std::byte>& decoded_data,
    TextEncoding encoding) {

    if (encoding == TextEncoding::UTF8) {
        return InterpretDecodedDataAsUTF8String(decoded_data, false).value_or(std::wstring{});
    }

    if (encoding == TextEncoding::UTF16) {
        return InterpretDecodedDataAsUTF16String(decoded_data);
    }

    return std::wstring{};
}

}