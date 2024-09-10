#include "module/tool/error/error_preview_control.h"
#include <zaf/base/string/case_conversion.h>
#include <zaf/base/string/to_string.h>
#include <zaf/base/string/trim.h>
#include <zaf/graphic/font.h>
#include <zaf/graphic/graphic_factory.h>
#include <zaf/graphic/dwrite/text_format_properties.h>
#include <zaf/graphic/dwrite/text_layout.h>
#include "module/common/style_constants.h"

namespace ra::mod::tool::error {
namespace {

constexpr float MaxFontSize = 26;
constexpr float MinFontSize = StyleConstants::PreservedBodyFontSize;

template<bool IsHistroical>
struct StyleMetrics;

template<>
struct StyleMetrics<false> {
    static constexpr auto HexAlignment = zaf::AxisAlignment::Center;
    static constexpr float ErrorMessageMinHeight = 50;
    
    static zaf::Frame MultilineErrorMessageMargin() {
        return zaf::Frame{};
    }

    static zaf::Frame Padding() {
        return zaf::Frame{ 0, 10, 0, 10 };
    }
};

template<>
struct StyleMetrics<true> {
    static constexpr auto HexAlignment = zaf::AxisAlignment::Start;
    static constexpr float ErrorMessageMinHeight = 30;

    static zaf::Frame MultilineErrorMessageMargin() {
        return zaf::Frame{ 0, 4, 0, 4 };
    }

    static zaf::Frame Padding() {
        return zaf::Frame{ 0, 2, 0, 0 };
    }
};

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

ZAF_OBJECT_IMPL(ErrorPreviewControl);

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


void ErrorPreviewControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    if (this->Width() != previous_rect.size.width) {
        AdjustLayout();
    }
}


void ErrorPreviewControl::OnStyleChanged() {

    errorMessage->Display(Style());
    AdjustLayout();
}


void ErrorPreviewControl::AdjustLayout() {

    bool is_multiline{};
    AdjustErrorMessageToFitContentSize(is_multiline);
    ChangeLayoutByStyle();
}


void ErrorPreviewControl::AdjustErrorMessageToFitContentSize(bool& is_multiline) {

    auto content_size = ContentSize();
    if (content_size.width <= 0) {
        return;
    }

    if (AdjustErrorMessageInSingleLine(content_size.width)) {
        is_multiline = false;
        return;
    }

    AdjustErrorMessageInMultilines(content_size.width);
    is_multiline = true;
}


bool ErrorPreviewControl::AdjustErrorMessageInSingleLine(float content_width) {

    const float max_font_size = Style() == CommandDisplayStyle::Preserved ? MinFontSize : MaxFontSize;

    bool has_fit{};
    float new_font_size{};
    for (new_font_size = max_font_size; new_font_size >= MinFontSize; --new_font_size) {

        errorMessage->SetFontSize(new_font_size);

        auto preferred_size = errorMessage->CalculatePreferredSize();
        if (preferred_size.width <= content_width) {
            has_fit = true;
            break;
        }
    }

    if (!has_fit) {
        return false;
    }

    errorMessage->SetFontSize(new_font_size);
    return has_fit;
}


void ErrorPreviewControl::AdjustErrorMessageInMultilines(float content_width) {

    auto update_gurad = this->BeginUpdate();

    errorMessage->SetFontSize(MinFontSize);
    errorMessage->SetIsMultiline(true);
    errorMessage->SetWordWrapping(zaf::dwrite::WordWrapping::Wrap);
    errorMessage->SetAllowVerticalScroll(false);
}


void ErrorPreviewControl::ChangeLayoutByStyle() {

    auto set_style = [this](auto metrics) {
    
        auto update_guard = this->BeginUpdate();

        this->SetPadding(metrics.Padding());
        hexContainer->SetAxisAlignment(metrics.HexAlignment);

        if (errorMessage->LineCount() <= 1) {
            errorMessage->SetFixedHeight(metrics.ErrorMessageMinHeight);
        }
        else {
            zaf::Size boundary{ ContentSize().width, std::numeric_limits<float>::max() };
            auto preferred_size = errorMessage->CalculatePreferredSize(boundary);

            float height = std::max(preferred_size.height, metrics.ErrorMessageMinHeight);
            errorMessage->SetFixedHeight(height);

            errorMessage->SetMargin(metrics.MultilineErrorMessageMargin());
        }
    };

    if (Style() == CommandDisplayStyle::Preserved) {
        set_style(StyleMetrics<true>{});
    }
    else {
        set_style(StyleMetrics<false>{});
    }
}


std::wstring ErrorPreviewControl::GetText() const {
    return errorMessage->Text();
}

}