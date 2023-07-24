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

namespace ra::mod::tool::error {
namespace {

constexpr float MaxFontSize = 26;
constexpr float MinFontSize = 16;

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
}


void ErrorPreviewControl::OnRectChanged(const zaf::RectChangedInfo& event_info) {

    __super::OnRectChanged(event_info);

    AdjustErrorMessageToFitContentSize();
}


void ErrorPreviewControl::OnStyleChanged() {

    errorMessage->Display(Style());

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

    zaf::TextFormatProperties text_format_properties;
    text_format_properties.font_size = MaxFontSize;
    text_format_properties.font_weight = zaf::FontWeight::Regular;
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

    const float max_font_size = Style() == PreviewStyle::Historical ? MinFontSize : MaxFontSize;

    bool has_fit{};
    float new_font_size{};
    for (new_font_size = max_font_size; new_font_size >= MinFontSize; --new_font_size) {

        text_layout.SetFontSize(new_font_size, zaf::Range{ 0, errorMessage->TextLength() });

        auto metrics = text_layout.GetMetrics();
        if (metrics.Width() <= content_width) {
            has_fit = true;
            break;
        }
    }

    errorMessage->SetFontSize(std::max(new_font_size, MinFontSize));
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