#include "utility/markdown/render/styled_text_box.h"
#include <zaf/creation.h>
#include <zaf/graphic/canvas.h>
#include <zaf/object/type_definition.h>

using namespace zaf::text_box;

namespace ra::utility::markdown::render {

ZAF_DEFINE_TYPE(StyledTextBox)
ZAF_DEFINE_TYPE_END;

void StyledTextBox::Initialize() {

    __super::Initialize();

    //StyledTextBox is disabled by default, users can enable it explicitly.
    this->SetIsEnabled(false);
    this->SetWordWrapping(zaf::WordWrapping::Wrap);
    this->SetIgnoreTailingWhiteSpaces(true);
    this->SetSelectionBackgroundColorPicker([this](const zaf::Control& control) {
        return 
            is_in_focus_context_ || this->IsFocused() ?
            zaf::Color::FromARGB(0x300078d7) :
            zaf::Color::FromARGB(0x30848484);
    });
}


void StyledTextBox::PaintTextBackground(
    zaf::Canvas& canvas, 
    const zaf::Rect& dirty_rect, 
    const zaf::TextLayout& text_layout, 
    const zaf::Rect& layout_rect) {

    {
        auto region_guard = canvas.PushRegion(layout_rect, layout_rect);

        for (const auto& each_pair : background_colors_) {
            PaintBackgroundColorAtRange(canvas, text_layout, each_pair.first, each_pair.second);
        }
    }

    __super::PaintTextBackground(canvas, dirty_rect, text_layout, layout_rect);
}


void StyledTextBox::PaintBackgroundColorAtRange(
    zaf::Canvas& canvas, 
    const zaf::TextLayout& text_layout, 
    const zaf::Range& range,
    const zaf::Color& color) const {

    auto range_metrics = text_layout.HitTestRange(range);
    for (const auto& metrics : range_metrics) {

        auto rect = metrics.Rect();
        rect.position.y += line_gap_;
        rect.size.height -= line_gap_;

        canvas.DrawRectangle(rect, color);
    }
}


void StyledTextBox::SetStyledText(const StyledText& styled_text) {

    this->SetText(styled_text.Text());

    for (const auto& each_style : styled_text.Styles()) {

        this->SetFontInRange(each_style.style.font, each_style.range);
        this->SetTextColorInRange(each_style.style.text_color, each_style.range);

        if (each_style.style.background_color) {

            background_colors_.push_back(std::make_pair(
                each_style.range, 
                *each_style.style.background_color));
        }
    }
}


void StyledTextBox::SetLineSpacingByParagraphStyle(const StyleConfig& style_config) {

    if (style_config.paragraph_config.line_gap == 0) {
        return;
    }

    line_gap_ = style_config.paragraph_config.line_gap;

    //We fail to find a proper way to calculate the default line height,
    //so we have to do it by creating a mock text box.
    auto default_line_height = [&style_config]() {
        auto text_box = zaf::Create<StyledTextBox>();
        text_box->SetPadding({});
        text_box->SetBorder({});
        text_box->SetFont(style_config.basic_config.font);
        text_box->SetText(L"H");
        return text_box->CalculatePreferredSize().height;
    }();

    auto new_line_height = default_line_height + style_config.paragraph_config.line_gap;

    this->SetLineSpacing(zaf::LineSpacing{
        zaf::LineSpacingMethod::Uniform,
        new_line_height,
        new_line_height * 0.8f,
    });
}


void StyledTextBox::SetSelectionByPositionRange(
    const composite::PositionRange& position_range,
    bool scroll_to_selection) {

    auto sorted = position_range.Sort();
    const auto& begin = sorted.first;
    const auto& end = sorted.second;

    //There is no intersection between the range and the text box, clear the selection.
    if (end.y < 0 || begin.y >= this->Height()) {
        this->SetSelectionRange(
            {},
            scroll_to_selection ? SelectionOption::ScrollToCaret : SelectionOption::NoScroll);
        return;
    }

    std::size_t begin_index{};
    if (begin.y < 0) {
        begin_index = 0;
    }
    else {
        begin_index = this->FindIndexAtPosition(begin);
    }

    std::size_t end_index{};
    if (end.y >= this->Height()) {
        end_index = this->TextLength();
    }
    else {
        end_index = this->FindIndexAtPosition(end);
    }

    auto min = std::min(begin_index, end_index);
    auto max = std::max(begin_index, end_index);
    zaf::Range selection_range{ min, max - min };

    auto selection_option =
        position_range.Begin() == begin ? 
        SelectionOption::SetCaretToEnd : 
        SelectionOption::SetCaretToBegin;

    if (scroll_to_selection) {
        selection_option |= SelectionOption::ScrollToCaret;
    }

    this->SetSelectionRange(selection_range, selection_option);
}


void StyledTextBox::SetIsInFocusContext(bool is_focused) {

    if (is_in_focus_context_ == is_focused) {
        return;
    }

    is_in_focus_context_ = is_focused;
    NeedRepaint();
}


bool StyledTextBox::IsPositionInsideTextBoundary(const zaf::Point& mouse_position) {
    return this->IsPositionInsideText(mouse_position);
}

void StyledTextBox::BeginSelection(const zaf::Point& position) {

}


void StyledTextBox::ChangeSelection(const composite::PositionRange& position_range) {
    SetSelectionByPositionRange(position_range, true);
}


void StyledTextBox::EndSelection(){

}


void StyledTextBox::SelectWord(const zaf::Point& position) {
    auto index = this->FindIndexAtPosition(position);
    this->SelectWordAtIndex(index);
}


void StyledTextBox::BuildSelectedText(composite::SelectedTextBuilder& builder) {
    auto selected_text = this->SelectedText();
    if (!selected_text.empty()) {
        builder.Append(selected_text);
    }
}


void StyledTextBox::ChangeFocus(bool is_focused) {
    this->SetIsInFocusContext(is_focused);
}

}