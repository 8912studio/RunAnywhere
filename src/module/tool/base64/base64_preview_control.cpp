#include "module/tool/base64/base64_preview_control.h"
#include <zaf/base/base64.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/type_definition.h>
#include "module/tool/base64/decoded_data_interpreting.h"

namespace ra::mod::tool::base64 {

ZAF_DEFINE_TYPE(Base64PreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/base64/base64_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void Base64PreviewControl::ShowParseResult(const Base64CommandParseResult& parse_result) {

    //Try to decode data first.
    std::optional<std::vector<std::byte>> decoded_data;
    bool can_decode{};
    if ((!parse_result.operation && !parse_result.input_text.empty()) || 
        (parse_result.operation && *parse_result.operation == Base64Operation::Decode)) {

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

    auto update_guard = this->BeginUpdate();

    operationLabel->SetText(operation == Base64Operation::Encode ? L"ENC" : L"DEC");
    operationLabel->SetBackgroundColor(zaf::Color::FromRGB(
        operation == Base64Operation::Encode ? 0x79A3EF : 0xF4A460
    ));

    if (operation == Base64Operation::Encode) {
        ShowEncodeResult(parse_result);
    }
    else if (decoded_data) {
        ShowDecodeResult(std::move(*decoded_data), parse_result);
    }
    else {
        //Fail to decode data.
    }
}


void Base64PreviewControl::ShowEncodeResult(const Base64CommandParseResult& parse_result) {

    std::string encoded_text;
    auto encoding = parse_result.encoding.value_or(TextEncoding::UTF8);
    if (encoding == TextEncoding::UTF8) {
        auto utf8_input = zaf::ToUTF8String(parse_result.input_text);
        encoded_text = zaf::Base64Encode(utf8_input.data(), utf8_input.size());
    }
    else {
        const auto& utf16_input = parse_result.input_text;
        encoded_text = zaf::Base64Encode(utf16_input.data(), utf16_input.size() * sizeof(wchar_t));
    }

    contentStatusBar->ShowText(parse_result.input_text, encoding);

    textContent->SetDisplayMode(TextDisplayMode::Base64);
    textContent->SetText(zaf::FromUTF8String(encoded_text));
    textContent->SetIsVisible(true);
}


void Base64PreviewControl::ShowDecodeResult(
    std::vector<std::byte> decoded_data,
    const Base64CommandParseResult& parse_result) {

    TextEncoding encoding{};
    auto text = ConvertDecodedDataToText(decoded_data, parse_result, encoding);
    if (text) {

        contentStatusBar->ShowText(parse_result.input_text, encoding);

        textContent->SetDisplayMode(TextDisplayMode::Normal);
        textContent->SetText(std::move(*text));
        textContent->SetIsVisible(true);
    }
    else {

        contentStatusBar->ShowBinary(parse_result.input_text);

        binaryContent->SetBinary(std::move(decoded_data));
        binaryContent->SetIsVisible(true);
    }
}


std::optional<std::wstring> Base64PreviewControl::ConvertDecodedDataToText(
    const std::vector<std::byte>& decoded_data,
    const Base64CommandParseResult& parse_result,
    TextEncoding& encoding) {

    if (parse_result.use_binary) {
        return std::nullopt;
    }

    if (parse_result.encoding) {
        encoding = *parse_result.encoding;
        return InterpretDecodedDataAsText(decoded_data, *parse_result.encoding);
    }

    return TryToInterpretDecodedDataAsText(decoded_data, encoding);
}


zaf::Frame Base64PreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.left = 0;
    result.right = 0;
    result.bottom = 0;
    return result;
}

}