#include "module/tool/base64/base64_preview_control.h"
#include <zaf/base/base64.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/type_definition.h>
#include "module/common/error_messages.h"
#include "module/tool/base64/decoded_data_interpreting.h"

namespace ra::mod::tool::base64 {
namespace {

template<bool IsHistorical>
struct StyleMetrics;

template<>
struct StyleMetrics<false> {
    static constexpr std::size_t BinaryLinesPerPage = 8;
};

template<>
struct StyleMetrics<true> {
    static constexpr std::size_t BinaryLinesPerPage = 2;
};

}

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

    ShowOperationType(operation);

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

    ShowTextContent(
        parse_result.input_text, 
        encoding,
        zaf::FromUTF8String(encoded_text), 
        true,
        L"Input is text",
        encoding == TextEncoding::UTF8 ? L"Input text is UTF-8" : L"Input text is UTF-16");
}


void Base64PreviewControl::ShowDecodeResult(
    std::vector<std::byte> decoded_data,
    const Base64CommandParseResult& parse_result) {

    TextEncoding encoding{};
    auto text = ConvertDecodedDataToText(decoded_data, parse_result, encoding);
    if (text) {
        ShowTextContent(
            parse_result.input_text, 
            encoding, 
            std::move(*text), 
            false,
            L"Display decoded result as text",
            encoding == TextEncoding::UTF8 ? L"Result text is UTF-8" : L"Result text is UTF-16");
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


void Base64PreviewControl::ShowOperationType(Base64Operation operation) {

    bool is_encode = operation == Base64Operation::Encode;

    operationLabel->SetText(is_encode ? L"ENC" : L"DEC");
    operationLabel->SetBackgroundColor(zaf::Color::FromRGB(is_encode ? 0x79A3EF : 0xF4A460));
    operationLabel->SetTooltip(is_encode ? L"Encode to Base64" : L"Decode from Base64");
}


void Base64PreviewControl::ShowTextContent(
    const std::wstring& input_text,
    TextEncoding encoding,
    std::wstring text_content,
    bool is_base64,
    const std::wstring& icon_tooltip,
    const std::wstring& encoding_tooltip) {

    contentStatusBar->ShowText(input_text, encoding);
    contentStatusBar->SetIconTooltip(icon_tooltip);
    contentStatusBar->SetEncodingTooltip(encoding_tooltip);

    if (!text_content.empty()) {

        TextDisplayMode display_mode;
        if (is_base64) {
            display_mode.use_fixed_width_font = true;
            display_mode.word_wrapping = zaf::WordWrapping::Character;
        }

        textContent->SetDisplayMode(display_mode);
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
    contentStatusBar->SetIconTooltip(L"Display decoded result as bytes");

    if (!binary_content.empty()) {
        binaryContent->SetBinary(std::move(binary_content));
        binaryContent->SetIsVisible(true);
    }
    else {
        errorView->ShowHintText(ErrorMessages::NoContentToDisplay);
        errorView->SetIsVisible(true);
    }
}


std::wstring Base64PreviewControl::GetResultText() const {

    if (textContent->IsVisible()) {
        return textContent->GetText();
    }
    return {};
}


void Base64PreviewControl::OnStyleChanged() {

    textContent->ChangeStyle(Style());
    errorView->ChangeStyle(Style());

    auto set_style = [this](auto metrics) {
        binaryContent->SetLinesPerPage(metrics.BinaryLinesPerPage);
    };

    if (Style() == PreviewStyle::Historical) {
        set_style(StyleMetrics<true>{});
    }
    else {
        set_style(StyleMetrics<false>());
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