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
constexpr float MinTextLayoutHeight = 80;

}

ZAF_DEFINE_TYPE(TextContentControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/text_content_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextContentControl::SetDisplayMode(TextDisplayMode mode) {

    display_mode_ = mode;
    textBox->SetFontFamily(mode == TextDisplayMode::Base64 ? L"Consolas" : L"");
}


void TextContentControl::SetText(const std::wstring& text) {

    textBox->SetText(text);
    has_line_break_ = utility::HasLineBreak(text);

    CalculateAndAdjustControls();
}


std::wstring TextContentControl::GetText() const {
    return textBox->Text();
}


void TextContentControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    CalculateAndAdjustControls();
}


void TextContentControl::CalculateAndAdjustControls() {

    //Ignore abnormal size.
    if (GetTextLayoutWidth() <= 0) {
        return;
    }

    if (!has_line_break_) {

        //Try to use single line mode if there is no line break in text.
        if (TryToAdjustForSingleLineText()) {
            return;
        }
    }

    //There are line breaks in text, or text is too long, use multi line mode.
    AdjustForMultiLineText();
}


bool TextContentControl::TryToAdjustForSingleLineText() {

    bool can_fit_in_single_line = DeterminateIfAllTextCanFitInSingleLine();
    if (!can_fit_in_single_line) {
        return false;
    }

    textBox->SetTextAlignment(zaf::TextAlignment::Center);

    SetControlHeight(MinTextLayoutHeight);
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


void TextContentControl::AdjustForMultiLineText() {

    textBox->SetTextAlignment(zaf::TextAlignment::Left);
    textBox->SetFontSize(MultiLineFontSize);

    if (display_mode_ == TextDisplayMode::Base64) {
        textBox->SetWordWrapping(zaf::WordWrapping::Character);
    }

    zaf::Size text_boundary_size{ GetTextLayoutWidth(), MinTextLayoutHeight };
    auto text_preferred_size = textBox->CalculatePreferredSize(text_boundary_size);

    auto edit_height = CalculateRequriedHeightForMultiLineEdit(
        text_preferred_size,
        textBox->LineCount(),
        text_boundary_size);

    SetControlHeight(edit_height);
}


void TextContentControl::SetControlHeight(float text_layout_height) {

    auto control_height = text_layout_height + scrollControl->HorizontalScrollBar()->Height();
    this->SetFixedHeight(control_height);
}


float TextContentControl::CalculateRequriedHeightForMultiLineEdit(
    const zaf::Size& text_preferrence_size,
    std::size_t line_count,
    const zaf::Size& text_bounds) {

    if (text_preferrence_size.height <= text_bounds.height) {
        return text_bounds.height;
    }

    constexpr std::size_t MaxShowLineCount = 10;

    std::size_t show_line_count{};
    if (line_count > MaxShowLineCount) {
        show_line_count = MaxShowLineCount;
    }
    else {
        show_line_count = line_count;
    }

    auto line_height = text_preferrence_size.height / line_count;
    return show_line_count * line_height;
}

}