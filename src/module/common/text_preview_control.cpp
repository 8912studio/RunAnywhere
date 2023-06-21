#include "module/common/text_preview_control.h"
#include <zaf/base/log.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>
#include "utility/text_utility.h"

namespace ra::mod {
namespace {

constexpr float MultiLineFontSize = 16;
constexpr float SingleLineMinFontSize = 18;
constexpr float SingleLineMaxFontSize = 26;
constexpr float MinControlHeight = 90;

}

ZAF_DEFINE_TYPE(TextPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/text_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextPreviewControl::SetText(const std::wstring& text) {

    if (text.empty()) {
        ShowEmptyText();
    }
    else {
        ShowText(text);
    }
}


void TextPreviewControl::SetWrapText(bool wrap) {
    wrap_text_ = wrap;
    CalculateAndAdjustControls();
}


void TextPreviewControl::ShowEmptyText() {

    scrollControl->SetIsVisible(false);
    richEdit->SetText({});

    emptyLabel->SetIsVisible(true);
}


void TextPreviewControl::ShowText(const std::wstring& text) {

    emptyLabel->SetIsVisible(false);
    scrollControl->SetIsVisible(true);

    auto update_guard = richEdit->BeginUpdate();

    richEdit->SetText(text);
    has_line_break_ = utility::HasLineBreak(text);

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

    if (!has_line_break_) {

        //Try to use single line mode if there is no line break in text.
        if (TryToAdjustForSingleLineText()) {
            return;
        }
    }

    //There are line breaks in text, or wrapping is set, use multi line mode.
    AdjustForMultiLineText();
}


bool TextPreviewControl::TryToAdjustForSingleLineText() {

    //Set rich edit to single line mode.
    richEdit->SetIsMultiline(false);
    richEdit->SetTextAlignment(zaf::TextAlignment::Center);

    //Find suitable font size.
    auto content_size = this->ContentSize();
    bool found_suitable_font_size{ false };
    for (float font_size = SingleLineMaxFontSize; 
         font_size >= SingleLineMinFontSize;
         --font_size) {

        richEdit->SetFontSize(font_size);
        auto edit_size = richEdit->CalculatePreferredSize();

        if (edit_size.width <= content_size.width) {
            found_suitable_font_size = true;
            break;
        }
    }

    //Cannot find suitable font size, but wrapping is set, switch to multi line mode.
    if (!found_suitable_font_size && wrap_text_) {
        return false;
    }

    scrollControl->SetAllowVerticalScroll(false);
    scrollControl->SetAllowHorizontalScroll(!found_suitable_font_size);
    this->SetFixedHeight(MinControlHeight);
    return true;
}


void TextPreviewControl::AdjustForMultiLineText() {

    richEdit->SetIsMultiline(true);
    richEdit->SetTextAlignment(zaf::TextAlignment::Left);
    richEdit->SetFontSize(MultiLineFontSize);
    richEdit->SetWordWrapping(wrap_text_ ? zaf::WordWrapping::Wrap : zaf::WordWrapping::NoWrap);
    
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