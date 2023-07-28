#include "module/common/error_view.h"
#include <zaf/creation.h>
#include <zaf/graphic/color.h>
#include <zaf/object/type_definition.h>

namespace ra::mod {
namespace {

template<bool IsHistorical>
struct StyleMetrics;

template<>
struct StyleMetrics<false> {
    static constexpr auto Alignment = zaf::AxisAlignment::Center;
    static constexpr auto IconSize = 48;
    static constexpr const wchar_t* IconURI = L"res:///resource/error.png";
    static constexpr float FontSize = 22;
    static constexpr float GapBetweenIconAndText = 10;
    static constexpr float Height = 90;
};

template<>
struct StyleMetrics<true> {
    static constexpr auto Alignment = zaf::AxisAlignment::Start;
    static constexpr auto IconSize = 20;
    static constexpr const wchar_t* IconURI = L"res:///resource/error_small.png";
    static constexpr float FontSize = 16;
    static constexpr float GapBetweenIconAndText = 6;
    static constexpr float Height = 28;
};

}

ZAF_DEFINE_TYPE(ErrorView)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/error_view.xaml")
ZAF_DEFINE_TYPE_END;

void ErrorView::ShowErrorText(const std::wstring& text) {
    ShowText(text, true);
}


void ErrorView::ShowHintText(const std::wstring& text) {
    ShowText(text, false);
}


void ErrorView::ShowText(const std::wstring& text, bool is_error) {

    auto update_guard = this->BeginUpdate();
    errorIcon->SetIsVisible(is_error);
    textLabel->SetText(text);
    textLabel->SetTextColor(is_error ? zaf::Color::Black() : zaf::Color::FromRGB(0xAAAAAA));
}


void ErrorView::ChangeStyle(PreviewStyle style) {

    auto set_style = [this](auto metrics) {
    
        auto update_guard = this->BeginUpdate();
        this->SetAxisAlignment(metrics.Alignment);
        errorIcon->SetFixedSize(zaf::Size{ metrics.IconSize, metrics.IconSize });
        errorIcon->SetURI(metrics.IconURI);
        errorIcon->SetMargin(zaf::Frame{ 0, 0, metrics.GapBetweenIconAndText, 0 });
        textLabel->SetFontSize(metrics.FontSize);
        this->SetFixedHeight(metrics.Height);
    };

    if (style == PreviewStyle::Historical) {
        set_style(StyleMetrics<true>{});
    }
    else {
        set_style(StyleMetrics<false>{});
    }
}

}