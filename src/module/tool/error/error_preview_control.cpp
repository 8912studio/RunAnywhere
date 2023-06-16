#include "module/tool/error/error_preview_control.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/to_string.h>
#include <zaf/base/string/trim.h>
#include <zaf/graphic/font/font.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/text/text_format_properties.h>
#include <zaf/graphic/text/text_layout.h>
#include <zaf/graphic/text/text_layout_properties.h>
#include <zaf/object/type_definition.h>

namespace ra::module::tool::error {
namespace {

std::wstring GetErrorMessage(std::uint32_t error_code) {

    wchar_t* buffer{};

    int format_flags =
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS | 
        FORMAT_MESSAGE_ALLOCATE_BUFFER;

    auto result_length = FormatMessage(
        format_flags,
        nullptr, 
        error_code,
        0,
        reinterpret_cast<LPWSTR>(&buffer),
        0,
        nullptr);

    std::wstring result;
    if (result_length > 0) {

        result.assign(buffer, result_length);
        zaf::Trim(result);

        LocalFree(buffer);
    }

    return result;
}

}

ZAF_DEFINE_TYPE(ErrorPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/error/error_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void ErrorPreviewControl::ShowErrorMessage(const ErrorCommandParseResult& parse_result) {

    auto hex = zaf::ToWideString(parse_result.error_code, zaf::ToStringOptions().Base(16));
    zaf::Uppercase(hex);

    hexErrorCode->SetText(L"0x" + hex);

    auto error_message = GetErrorMessage(parse_result.error_code);
    if (!error_message.empty()) {
        errorMessage->SetText(error_message);
        errorMessage->SetTextColor(zaf::Color::Black());
    }
    else {
        errorMessage->SetText(L"No error message found");
        errorMessage->SetTextColor(zaf::Color::FromRGB(0x909090));
    }

    AdjustErrorMessageToFitContentSize();
}


void ErrorPreviewControl::OnRectChanged(const zaf::RectChangedInfo& event_info) {

    __super::OnRectChanged(event_info);

    AdjustErrorMessageToFitContentSize();
}


void ErrorPreviewControl::AdjustErrorMessageToFitContentSize() {

    auto content_size = ContentSize();
    if (content_size.width <= 0) {
        return;
    }

    auto text_layout = CreateTextLayoutForMeasuring();

    if (AdjustErrorMessageByReducingFontSize(text_layout, content_size.width)) {
        return;
    }

    AdjustErrorMessageByAddingLines(text_layout, content_size.width);
}


zaf::TextLayout ErrorPreviewControl::CreateTextLayoutForMeasuring() const {

    auto font = errorMessage->Font();
    zaf::TextFormatProperties text_format_properties;
    text_format_properties.font_family_name = font.family_name;
    text_format_properties.font_size = font.size;
    text_format_properties.font_weight = font.weight;
    auto text_format = zaf::GraphicFactory::Instance().CreateTextFormat(text_format_properties);

    zaf::TextLayoutProperties text_layout_properties;
    text_layout_properties.text = errorMessage->Text();
    text_layout_properties.width = std::numeric_limits<float>::max();
    text_layout_properties.height = std::numeric_limits<float>::max();
    text_layout_properties.text_format = text_format;
    return zaf::GraphicFactory::Instance().CreateTextLayout(text_layout_properties);
}


bool ErrorPreviewControl::AdjustErrorMessageByReducingFontSize(
    zaf::TextLayout& text_layout,
    float content_width) {

    float new_font_size = errorMessage->FontSize();
    bool has_fit{};

    while (new_font_size > 16) {

        auto metrics = text_layout.GetMetrics();
        if (metrics.Width() <= content_width) {
            has_fit = true;
            break;
        }

        new_font_size -= 1;
        text_layout.SetFontSize(new_font_size, zaf::TextRange{ 0, errorMessage->TextLength() });
    }

    errorMessage->SetFontSize(new_font_size);
    return has_fit;
}


void ErrorPreviewControl::AdjustErrorMessageByAddingLines(
    zaf::TextLayout& text_layout, 
    float content_width) {

    auto update_gurad = this->BeginUpdate();

    errorMessage->SetIsMultiline(true);
    errorMessage->SetWordWrapping(zaf::WordWrapping::Wrap);
    errorMessage->SetAllowVerticalScroll(false);

    auto margin = errorMessage->Margin();
    margin.top = 8;
    errorMessage->SetMargin(margin);

    text_layout.SetMaxWidth(content_width);
    text_layout.SetWordWrapping(zaf::WordWrapping::Wrap);
    auto metrics = text_layout.GetMetrics();
    if (metrics.LineCount() > 2) {
        errorMessage->SetFixedHeight(metrics.Height());
    }
    else {
        errorMessage->SetFixedHeight(errorMessage->Height() - margin.top);
    }
}


std::wstring ErrorPreviewControl::GetText() const {
    return errorMessage->Text();
}

}