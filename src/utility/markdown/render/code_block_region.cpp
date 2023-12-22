#include "utility/markdown/render/code_block_region.h"
#include <zaf/base/log.h>
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/creation.h>

namespace ra::utility::markdown::render {

std::shared_ptr<CodeBlockRegion> CodeBlockRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::CodeBlock);

    auto result = zaf::Init(new CodeBlockRegion());

    if (!element.Children().empty()) {
        TextStyle text_style;
        text_style.font = style_config.basic_config.font;
        text_style.font.family_name = style_config.code_block_config.font_family_name;
        text_style.text_color = style_config.code_block_config.text_color;
        result->SetStyledText(element.Children().front()->Text(), text_style);
    }

    result->SetTextBackgroundColor(style_config.code_block_config.background_color);
    return result;
}


void CodeBlockRegion::Initialize() {

    __super::Initialize();

    SetPadding(zaf::Frame{ 10, 10, 10, 2 });
    SetLayouter(zaf::Create<zaf::VerticalLayouter>());

    scroll_control_ = zaf::Create<utility::ThinScrollControl>();
    scroll_control_->SetAllowVerticalScroll(false);
    scroll_control_->SetAutoHideScrollBars(true);
    AddChild(scroll_control_);

    text_box_ = zaf::Create<StyledTextBox>();
    text_box_->SetIsEnabled(false);
    text_box_->SetPadding(zaf::Frame{ 0, 0, 0, 8 });
    text_box_->SetWordWrapping(zaf::WordWrapping::NoWrap);
    scroll_control_->SetScrollContent(text_box_);
}


zaf::Size CodeBlockRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    auto result = text_box_->CalculatePreferredSize(bound_size);
    if (result.width > bound_size.width) {
        result.height += scroll_control_->HorizontalScrollBar()->Height();
    }
    return result;
}


void CodeBlockRegion::SetStyledText(const std::wstring& text, const TextStyle& text_style) {
    text_box_->SetText(text);
    text_box_->SetFont(text_style.font);
    text_box_->SetTextColor(text_style.text_color);
}


void CodeBlockRegion::SetTextBackgroundColor(const zaf::Color& color) {

    this->SetBackgroundColor(color);
    text_box_->SetBackgroundColor(color);
}


bool CodeBlockRegion::ChangeMouseCursor(const zaf::Point& mouse_position) {

    auto position_in_text_box = this->TranslatePositionToChild(mouse_position, *scroll_control_);

    if (text_box_->RectInSelf().Contain(position_in_text_box)) {
        return text_box_->TryToChangeMouseCursor(position_in_text_box);
    }

    return false;
}


void CodeBlockRegion::BeginSelection(const zaf::Point& position) {

    if (!IsPositionInTextBox(position)) {
        return;
    }

    auto horizontal_scroll_bar = scroll_control_->HorizontalScrollBar();
    begin_selection_x_offset_ = static_cast<float>(horizontal_scroll_bar->Value());
}


void CodeBlockRegion::ChangeSelection(const PositionRange& position_range) {

    auto begin_position_in_text_box =
        this->TranslatePositionToChild(position_range.Begin(), *scroll_control_);

    bool scroll_to_selection{ false };
    if (begin_selection_x_offset_) {

        auto horizontal_scroll_bar = scroll_control_->HorizontalScrollBar();
        auto current_x_offset = static_cast<float>(horizontal_scroll_bar->Value());

        begin_position_in_text_box.x -= current_x_offset - *begin_selection_x_offset_ ;

        //If both begin and end position are in the rectangle of the text box, scroll to the 
        //selection.
        if (IsPositionInTextBox(position_range.End())) {
            scroll_to_selection = true;
        }
    }

    PositionRange position_range_in_text_box{ 
        begin_position_in_text_box,
        this->TranslatePositionToChild(position_range.End(), *scroll_control_) 
    };
    text_box_->SetSelectionByPositionRange(position_range_in_text_box, scroll_to_selection);
}


bool CodeBlockRegion::IsPositionInTextBox(const zaf::Point& position) const {

    auto position_in_text_box = this->TranslatePositionToChild(position, *scroll_control_);
    return 
        (position_in_text_box.y >= 0) && 
        (position_in_text_box.y < text_box_->Height());
}


void CodeBlockRegion::EndSelection() {
    begin_selection_x_offset_.reset();
}


void CodeBlockRegion::SelectWord(const zaf::Point& position) {

    if (!IsPositionInTextBox(position)) {
        return;
    }

    auto position_in_text_box = this->TranslatePositionToChild(position, *scroll_control_);
    auto index = text_box_->FindIndexAtPosition(position_in_text_box);
    text_box_->SelectWordAtIndex(index);
}


void CodeBlockRegion::BuildSelectedText(SelectedTextBuilder& builder) {

    auto selected_text = text_box_->SelectedText();
    if (!selected_text.empty()) {
        builder.Append(selected_text);
    }
}


void CodeBlockRegion::ChangeFocus(bool is_focused) {
    text_box_->SetIsInFocusContext(is_focused);
}

}