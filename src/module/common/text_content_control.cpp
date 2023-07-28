#include "module/common/text_content_control.h"
#include <zaf/base/container/utility/erase.h>
#include <zaf/base/log.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>
#include "utility/text_utility.h"

namespace ra::mod {
namespace {

constexpr float MultiLineFontSize = 16;
constexpr float SingleLineMinFontSize = MultiLineFontSize;
constexpr float SingleLineMaxFontSize = 26;
constexpr std::size_t SingleLineMaxCalculateLength = 100;

constexpr float NormalStyleMinTextLayoutHeight = 76;
constexpr std::size_t NormalStyleMaxShowLineCount = 10;

zaf::Frame NormalStylePadding() {
    return zaf::Frame{ 0, 4, 0, 0 };
}

constexpr float HistoricalStyleMinTextLayoutHeight = 24;
constexpr std::size_t HistoricalStyleMaxShowLineCount = 2;

zaf::Frame HistoricalStylePadding() {
    return zaf::Frame{};
}

}

ZAF_DEFINE_TYPE(TextContentControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/text_content_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextContentControl::SetDisplayMode(const TextDisplayMode& mode) {

    display_mode_ = mode;
    textBox->SetFontFamily(
        mode.use_fixed_width_font ? L"Consolas" : zaf::Font::Default().family_name);

    AdjustLayout();
}


void TextContentControl::SetText(const std::wstring& text) {

    textBox->SetText(text);
    has_line_break_ = utility::HasLineBreak(text);

    AdjustLayout();
}


std::wstring TextContentControl::GetText() const {
    return textBox->Text();
}


void TextContentControl::ChangeStyle(PreviewStyle style) {

    style_ = style;

    auto update_guard = this->BeginUpdate();

    this->SetPadding(
        style_ == PreviewStyle::Historical ? HistoricalStylePadding() : NormalStylePadding());

    AdjustLayout();
}


void TextContentControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    AdjustLayout();
}


void TextContentControl::AdjustLayout() {

    //Ignore abnormal size.
    if (GetTextLayoutWidth() <= 0) {
        return;
    }

    auto layout_info = AdjustTextLayout();

    float control_height = layout_info.required_height;
    if (style_ == PreviewStyle::Historical && !layout_info.need_horizontal_scroll) {
        scrollControl->SetAllowHorizontalScroll(false);
    }
    else {
        scrollControl->SetAllowHorizontalScroll(true);
        control_height += scrollControl->HorizontalScrollBar()->Height();
    }

    control_height += this->Padding().Height();
    this->SetFixedHeight(control_height);
}


TextContentControl::LayoutInfo TextContentControl::AdjustTextLayout() {

    if (!has_line_break_ && (style_ != PreviewStyle::Historical)) {

        //Try to use single line mode if there is no line break in text.
        if (TryToAdjustForSingleLineText()) {
            LayoutInfo layout_info;
            layout_info.required_height = GetMinTextHeight();
            layout_info.need_horizontal_scroll = false;
            return layout_info;
        }
    }

    //There are line breaks in text, or text is too long, use multi line mode.
    return AdjustForMultiLineText();
}


bool TextContentControl::TryToAdjustForSingleLineText() {

    bool can_fit_in_single_line = DeterminateIfAllTextCanFitInSingleLine();
    if (!can_fit_in_single_line) {
        return false;
    }

    textBox->SetTextAlignment(zaf::TextAlignment::Center);
    return true;
}


bool TextContentControl::DeterminateIfAllTextCanFitInSingleLine() {

    //It's no need to determine if the text is too long.
    if (textBox->TextLength() > SingleLineMaxCalculateLength) {
        return false;
    }

    //Find suitable font size.
    float layout_width = GetTextLayoutWidth();

    for (float font_size = SingleLineMaxFontSize;
        font_size >= SingleLineMinFontSize;
        --font_size) {

        textBox->SetFontSize(font_size);
        auto edit_size = textBox->CalculatePreferredSize();

        if (edit_size.width <= layout_width) {
            return true;
        }
    }

    return false;
}


float TextContentControl::GetTextLayoutWidth() const {

    auto content_size = this->ContentSize();
    auto width = content_size.width - scrollControl->VerticalScrollBar()->Width();
    return std::max(width, 0.f);
}


TextContentControl::LayoutInfo TextContentControl::AdjustForMultiLineText() {

    textBox->SetTextAlignment(zaf::TextAlignment::Left);
    textBox->SetFontSize(MultiLineFontSize);
    textBox->SetWordWrapping(display_mode_.word_wrapping);

    zaf::Size text_boundary_size{ GetTextLayoutWidth(), GetMinTextHeight() };
    auto text_preferred_size = textBox->CalculatePreferredSize(text_boundary_size);
    auto max_show_line_count = 
        style_ == PreviewStyle::Historical ?
        HistoricalStyleMaxShowLineCount :
        NormalStyleMaxShowLineCount;

    auto required_height = CalculateRequriedHeightForMultiLineEdit(
        text_preferred_size,
        textBox->LineCount(),
        text_boundary_size,
        max_show_line_count);

    LayoutInfo layout_info;
    layout_info.required_height = required_height;
    layout_info.need_horizontal_scroll = text_preferred_size.width > text_boundary_size.width;
    return layout_info;
}


float TextContentControl::GetMinTextHeight() const {

    return style_ == PreviewStyle::Historical ?
        HistoricalStyleMinTextLayoutHeight :
        NormalStyleMinTextLayoutHeight;
}


float TextContentControl::CalculateRequriedHeightForMultiLineEdit(
    const zaf::Size& text_preferrence_size,
    std::size_t line_count,
    const zaf::Size& text_bounds,
    std::size_t max_show_line_count) {

    if (text_preferrence_size.height <= text_bounds.height) {
        return text_bounds.height;
    }

    std::size_t show_line_count{};
    if (line_count > max_show_line_count) {
        show_line_count = max_show_line_count;
    }
    else {
        show_line_count = line_count;
    }

    auto line_height = text_preferrence_size.height / line_count;
    return show_line_count * line_height;
}

}