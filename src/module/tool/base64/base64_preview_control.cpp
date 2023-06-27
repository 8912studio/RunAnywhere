#include "module/tool/base64/base64_preview_control.h"
#include <zaf/base/base64.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/type_definition.h>
#include "module/common/error_messages.h"
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
        contentStatusBar->ShowNone(parse_result.input_text);
        errorView->ShowErrorText(L"Decode failed");
        errorView->SetIsVisible(true);
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

    ShowTextContent(parse_result.input_text, encoding, zaf::FromUTF8String(encoded_text), true);
}


void Base64PreviewControl::ShowDecodeResult(
    std::vector<std::byte> decoded_data,
    const Base64CommandParseResult& parse_result) {

    TextEncoding encoding{};
    auto text = ConvertDecodedDataToText(decoded_data, parse_result, encoding);
    if (text) {
        ShowTextContent(parse_result.input_text, encoding, std::move(*text), false);
    }
    else {
        ShowBinaryContent(parse_result.input_text, std::move(decoded_data));
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


void Base64PreviewControl::ShowTextContent(
    const std::wstring& input_text,
    TextEncoding encoding,
    std::wstring text_content,
    bool is_base64) {

    contentStatusBar->ShowText(input_text, encoding);

    if (!text_content.empty()) {
        textContent->SetDisplayMode(is_base64 ? TextDisplayMode::Base64 : TextDisplayMode::Normal);
        textContent->SetText(std::move(text_content));
        textContent->SetIsVisible(true);
    }
    else {
        errorView->ShowHintText(ErrorMessages::NoContentToDisplay);
        errorView->SetIsVisible(true);
    }
}


void Base64PreviewControl::ShowBinaryContent(
    const std::wstring& input_text,
    std::vector<std::byte> binary_content) {

    contentStatusBar->ShowBinary(input_text);

    if (!binary_content.empty()) {
        binaryContent->SetBinary(std::move(binary_content));
        binaryContent->SetIsVisible(true);
    }
    else {
        errorView->ShowHintText(ErrorMessages::NoContentToDisplay);
        errorView->SetIsVisible(true);
    }
}


zaf::Frame Base64PreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.left = 0;
    result.right = 0;
    result.bottom = 0;
    return result;
}

}