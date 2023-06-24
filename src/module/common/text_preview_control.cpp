#include "module/common/text_preview_control.h"
#include <zaf/base/container/utility/range.h>
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
constexpr float MinControlHeight = 90;

}

ZAF_DEFINE_TYPE(TextPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/text_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextPreviewControl::SetDisplayMode(DisplayMode mode) {
    display_mode_ = mode;
}


void TextPreviewControl::SetText(std::wstring text) {

    text_ = std::move(text);
    has_line_break_ = utility::HasLineBreak(text_);

    if (text_.empty()) {
        ShowEmptyText();
    }
    else {
        ShowText();
    }
}


void TextPreviewControl::ShowEmptyText() {

    scrollControl->SetIsVisible(false);
    richEdit->SetText({});

    emptyLabel->SetIsVisible(true);
}


void TextPreviewControl::ShowText() {

    emptyLabel->SetIsVisible(false);
    scrollControl->SetIsVisible(true);

    auto update_guard = richEdit->BeginUpdate();

    auto font = richEdit->Font();
    font.family_name = display_mode_ == DisplayMode::Base64 ? L"Consolas" : L"";
    richEdit->SetFont(font);

    CalculateAndAdjustControls();
}


zaf::Frame TextPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    //Use thickness of scroll bars as margins.
    result.right = 0;
    result.bottom = 0;
    return result;
}


void TextPreviewControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    CalculateAndAdjustControls();
}


void TextPreviewControl::CalculateAndAdjustControls() {

    //Ignore abnormal size.
    if (GetTextLayoutWidth() <= 0) {
        return;
    }

    bool has_set_text{};
    if (!has_line_break_) {

        //Try to use single line mode if there is no line break in text.
        if (TryToAdjustForSingleLineText()) {
            return;
        }

        if (display_mode_ == DisplayMode::Base64) {
            BreakTextForBase64Mode();
            has_set_text = true;
        }
    }

    //There are line breaks in text, or wrapping is set, use multi line mode.
    AdjustForMultiLineText(has_set_text);
}


bool TextPreviewControl::TryToAdjustForSingleLineText() {

    //Set rich edit to single line mode first.
    richEdit->SetIsMultiline(false);

    bool has_set_text{};
    bool can_fit_in_single_line = DeterminateIfAllTextCanFitInSingleLine(has_set_text);

    //We use single line if:
    //1. All text can fit in single line.
    //2. Or display mode is set to normal, in such case horizontal scroll bar is shown.
    bool use_single_line = can_fit_in_single_line || display_mode_ == DisplayMode::Normal;
    if (!use_single_line) {
        return false;
    }

    if (!has_set_text) {
        richEdit->SetFontSize(SingleLineMinFontSize);
        richEdit->SetText(text_);
    }

    richEdit->SetTextAlignment(zaf::TextAlignment::Center);

    this->SetFixedHeight(MinControlHeight);
    return true;
}


bool TextPreviewControl::DeterminateIfAllTextCanFitInSingleLine(bool& has_set_text) {

    //We cannot use single line mode if the text is too long.
    if (text_.length() > SingleLineMaxCalculateLength) {
        return false;
    }

    has_set_text = true;

    //Find suitable font size.
    richEdit->SetText(text_);
    
    float layout_width = GetTextLayoutWidth();

    for (float font_size = SingleLineMaxFontSize;
        font_size >= SingleLineMinFontSize;
        --font_size) {

        richEdit->SetFontSize(font_size);
        auto edit_size = richEdit->CalculatePreferredSize();

        if (edit_size.width <= layout_width) {
            return true;
        }
    }

    return false;
}


float TextPreviewControl::GetTextLayoutWidth() const {

    auto content_size = this->ContentSize();
    auto width = content_size.width - scrollControl->VerticalScrollBar()->Width();
    return std::max(width, 0.f);
}


void TextPreviewControl::BreakTextForBase64Mode() {

    auto calculated_text = text_.substr(0, SingleLineMaxCalculateLength);

    richEdit->SetFontSize(SingleLineMinFontSize);
    richEdit->SetText(calculated_text);

    auto preferred_size = richEdit->CalculatePreferredSize();
    preferred_size.width -= richEdit->Padding().Width();
    preferred_size.width -= richEdit->Border().Width();
    float average_char_width = preferred_size.width / calculated_text.length();

    auto line_length =
        static_cast<std::size_t>(std::floor(GetTextLayoutWidth() / average_char_width));

    std::wstring new_text;
    new_text.reserve(text_.length() + (text_.length() / line_length * 2));
    for (auto index : zaf::Range(text_.length())) {

        if (index != 0 && (index % line_length == 0)) {
            new_text += L"\r\n";
        }
        
        new_text += text_[index];
    }

    richEdit->SetIsMultiline(true);
    richEdit->SetText(new_text);
}


void TextPreviewControl::AdjustForMultiLineText(bool has_set_text) {

    if (!has_set_text) {
        richEdit->SetText(text_);
    }

    richEdit->SetIsMultiline(true);
    richEdit->SetTextAlignment(zaf::TextAlignment::Left);
    richEdit->SetFontSize(MultiLineFontSize);
    
    auto control_height = CalculateRequriedHeightForMultiLineEdit(
        richEdit->CalculatePreferredSize(),
        richEdit->LineCount(),
        zaf::Size{ GetTextLayoutWidth(), MinControlHeight });

    control_height += scrollControl->HorizontalScrollBar()->Height();
    this->SetFixedHeight(control_height);
}


float TextPreviewControl::CalculateRequriedHeightForMultiLineEdit(
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