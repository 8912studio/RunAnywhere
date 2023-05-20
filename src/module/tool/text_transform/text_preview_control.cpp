#include "module/tool/text_transform/text_preview_control.h"
#include <zaf/base/log.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>
#include "utility/text_utility.h"

namespace ra::module::tool::text_transform {
namespace {

constexpr float MultiLineFontSize = 16;
constexpr float SingleLineMinFontSize = 18;
constexpr float SingleLineMaxFontSize = 26;
constexpr float MinControlHeight = 90;

}

ZAF_DEFINE_TYPE(TextPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/text_transform/text_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextPreviewControl::SetText(const std::wstring& text) {

    auto update_guard = richEdit->BeginUpdate();

    if (utility::HasLineBreak(text)) {
        richEdit->SetIsMultiline(true);
        richEdit->SetTextAlignment(zaf::TextAlignment::Left);
        richEdit->SetFontSize(MultiLineFontSize);
    }
    else {
        richEdit->SetIsMultiline(false);
        richEdit->SetTextAlignment(zaf::TextAlignment::Center);
        richEdit->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    }

    richEdit->SetText(text);
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

    if (richEdit->IsMultiline()) {
        AdjustForMultiLineEdit();
    }
    else {
        AdjustForSingleLineEdit();
    }
}


void TextPreviewControl::AdjustForSingleLineEdit() {

    auto content_size = this->ContentSize();
    if (content_size.width <= 0) {
        return;
    }

    bool need_horizontal_scorll_bar{ true };
    for (float font_size = SingleLineMaxFontSize; 
         font_size >= SingleLineMinFontSize;
         --font_size) {

        richEdit->SetFontSize(font_size);
        auto edit_size = richEdit->CalculatePreferredSize();

        if (edit_size.width <= content_size.width) {
            need_horizontal_scorll_bar = false;
            break;
        }
    }

    scrollControl->SetAllowVerticalScroll(false);
    scrollControl->SetAllowHorizontalScroll(need_horizontal_scorll_bar);
    this->SetFixedHeight(MinControlHeight);
}


void TextPreviewControl::AdjustForMultiLineEdit() {

    auto content_size = this->ContentSize();
    if (content_size.width <= 0) {
        return;
    }

    auto edit_size = richEdit->CalculatePreferredSize();
    auto line_count = richEdit->LineCount();
    zaf::Size bounds{ content_size.width, MinControlHeight };
    bool need_horizontal_scroll{};
    bool need_vertical_scroll{};
    auto control_height = CalculateRequriedHeightForMultiLineEdit(
        edit_size, 
        line_count, 
        bounds,
        need_horizontal_scroll,
        need_vertical_scroll);

    if (need_horizontal_scroll || need_vertical_scroll) {

        if (need_horizontal_scroll) {
            bounds.height -= scrollControl->HorizontalScrollBar()->Height();
        }

        if (need_vertical_scroll) {
            bounds.width -= scrollControl->VerticalScrollBar()->Width();
        }

        control_height = CalculateRequriedHeightForMultiLineEdit(
            edit_size, 
            line_count, 
            bounds, 
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
    const zaf::Size& edit_size,
    std::size_t line_count,
    const zaf::Size& bounds,
    bool& need_horizontal_scroll,
    bool& need_vertical_scroll) {

    need_horizontal_scroll = edit_size.width > bounds.width;

    if (edit_size.height <= bounds.height) {

        need_vertical_scroll = false;
        return bounds.height;
    }

    auto line_height = edit_size.height / line_count;

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