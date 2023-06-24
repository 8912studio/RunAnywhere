#include "module/tool/base64/base64_preview_control.h"
#include <zaf/base/base64.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/type_definition.h>

namespace ra::mod::tool::base64 {

ZAF_DEFINE_TYPE(Base64PreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/base64/base64_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void Base64PreviewControl::ShowParseResult(const Base64CommandParseResult& parse_result) {

    //Try to decode data first.
    std::optional<std::vector<std::byte>> decoded_data;
    bool can_decode{};
    if (!parse_result.operation || *parse_result.operation == Base64Operation::Decode) {
        try {
            decoded_data = zaf::Base64Decode(zaf::ToUTF8String(parse_result.input_text));
            can_decode = true;
        }
        catch (const zaf::Error&) {

        }
    }

    //Determinate which operation we should perform.
    //Use specified operation if there is. Or use decode if the data can be decoded, otherwise use 
    //encode.
    Base64Operation operation =
        parse_result.operation ? *parse_result.operation :
        can_decode ? Base64Operation::Decode : Base64Operation::Encode;

    if (operation == Base64Operation::Encode) {
        ShowEncodeResult(parse_result);
    }
    else if (decoded_data) {

    }
    else {
        //Fail to decode data.
    }
}


void Base64PreviewControl::ShowEncodeResult(const Base64CommandParseResult& parse_result) {

    std::string encoded_text;
    if (parse_result.encoding == TextEncoding::UTF8) {
        auto utf8_input = zaf::ToUTF8String(parse_result.input_text);
        encoded_text = zaf::Base64Encode(utf8_input.data(), utf8_input.size());
    }
    else {
        const auto& utf16_input = parse_result.input_text;
        encoded_text = zaf::Base64Encode(utf16_input.data(), utf16_input.size() * sizeof(wchar_t));
    }

    textContent->SetDisplayMode(TextDisplayMode::Base64);
    textContent->SetText(zaf::FromUTF8String(encoded_text));
    textContent->SetIsVisible(true);
}


zaf::Frame Base64PreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.right = 0;
    result.bottom = 0;
    return result;
}

}