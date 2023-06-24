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

    if (scrollControl->AllowVerticalScroll()) {
        result.right = 0;
    }

    if (scrollControl->AllowHorizontalScroll()) {
        result.bottom = 0;
    }

    return result;
}


void TextPreviewControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    CalculateAndAdjustControls();
}


void TextPreviewControl::CalculateAndAdjustControls() {

    //Ignore abnormal size.
    if (this->ContentSize().width <= 0) {
        return;
    }

    //Disable scroll bars by default, in order to make GetExpectedMargin() get correct values.
    scrollControl->SetAllowVerticalScroll(false);
    scrollControl->SetAllowHorizontalScroll(false);

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

    scrollControl->SetAllowVerticalScroll(false);
    scrollControl->SetAllowHorizontalScroll(!can_fit_in_single_line);
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
    
    auto content_size = this->ContentSize();

    for (float font_size = SingleLineMaxFontSize;
        font_size >= SingleLineMinFontSize;
        --font_size) {

        richEdit->SetFontSize(font_size);
        auto edit_size = richEdit->CalculatePreferredSize();

        if (edit_size.width <= content_size.width) {
            return true;
        }
    }

    return false;
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
        static_cast<std::size_t>(std::floor(this->ContentSize().width / average_char_width));

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
    
    auto content_size = this->ContentSize();
    zaf::Size text_bounds{ content_size.width, MinControlHeight };

    auto text_preferrence_size = richEdit->CalculatePreferredSize(zaf::Size{ 
        content_size.width, 
        std::numeric_limits<float>::max()
    });
    auto line_count = richEdit->LineCount();
    auto line_height = text_preferrence_size.height / line_count;

    bool need_horizontal_scroll{};
    bool need_vertical_scroll{};
    auto control_height = CalculateRequriedHeightForMultiLineEdit(
        text_preferrence_size, 
        line_count, 
        line_height,
        text_bounds,
        need_horizontal_scroll,
        need_vertical_scroll);

    if (need_horizontal_scroll || need_vertical_scroll) {

        if (need_horizontal_scroll) {
            text_bounds.height -= scrollControl->HorizontalScrollBar()->Height();
        }

        if (need_vertical_scroll) {

            text_bounds.width -= scrollControl->VerticalScrollBar()->Width();

            //Recalculate edit size when the width of text_bounds is changed.
            text_preferrence_size = richEdit->CalculatePreferredSize(zaf::Size{
                text_bounds.width,
                std::numeric_limits<float>::max()
            });
        }

        control_height = CalculateRequriedHeightForMultiLineEdit(
            text_preferrence_size, 
            line_count, 
            line_height,
            text_bounds, 
            need_horizontal_scroll, 
            need_vertical_scroll);
    }

    if (need_horizontal_scroll) {
        control_height += scrollControl->HorizontalScrollBar()->Height();
    }

    scrollControl->SetAllowHorizontalScroll(need_horizontal_scroll);
    scrollControl->SetAllowVerticalScroll(need_vertical_scroll);
    this->SetFixedHeight(control_height);
}


float TextPreviewControl::CalculateRequriedHeightForMultiLineEdit(
    const zaf::Size& text_preferrence_size,
    std::size_t line_count,
    float line_height,
    const zaf::Size& text_bounds,
    bool& need_horizontal_scroll,
    bool& need_vertical_scroll) {

    need_horizontal_scroll = text_preferrence_size.width > text_bounds.width;

    if (text_preferrence_size.height <= text_bounds.height) {

        need_vertical_scroll = false;
        return text_bounds.height;
    }

    constexpr std::size_t MaxShowLineCount = 10;

    std::size_t show_line_count{};
    if (line_count > MaxShowLineCount) {

        show_line_count = MaxShowLineCount;
        need_vertical_scroll = true;
    }
    else {
        show_line_count = line_count;
        need_vertical_scroll = false;
    }

    return show_line_count * line_height;
}

}