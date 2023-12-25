#include "utility/markdown/render/simple_block_region.h"
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include <zaf/graphic/canvas.h>
#include "utility/markdown/render/styled_text_builder.h"

namespace ra::utility::markdown::render {

void SimpleBlockRegion::Initialize() {

    __super::Initialize();

    SetLayouter(zaf::Create<zaf::VerticalLayouter>());

    text_box_ = zaf::Create<StyledTextBox>();
    AddChild(text_box_);
}


void SimpleBlockRegion::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);
}


zaf::Size SimpleBlockRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {
    return text_box_->CalculatePreferredSize(bound_size);
}


void SimpleBlockRegion::InitializeContent(
    const element::Element& element,
    const StyleConfig& style_config) {

    StyledTextBuilder styled_text_builder;
    auto styled_text = styled_text_builder.Build(element, style_config);

    text_box_->SetStyledText(styled_text);
}


bool SimpleBlockRegion::IsPositionInsideText(const zaf::Point& mouse_position) {
    return text_box_->IsPositionInsideText(
        this->TranslatePositionToChild(mouse_position, *text_box_));
}


void SimpleBlockRegion::ChangeSelection(const PositionRange& position_range) {

    text_box_->SetSelectionByPositionRange(PositionRange{
        this->TranslatePositionToChild(position_range.Begin(), *text_box_),
        this->TranslatePositionToChild(position_range.End(), *text_box_)
    });
}


void SimpleBlockRegion::SelectWord(const zaf::Point& position) {

    auto position_in_text_box = this->TranslatePositionToChild(position, *text_box_);
    auto index = text_box_->FindIndexAtPosition(position_in_text_box);
    text_box_->SelectWordAtIndex(index);
}


void SimpleBlockRegion::BuildSelectedText(SelectedTextBuilder& builder) {

    auto selected_text = text_box_->SelectedText();
    if (!selected_text.empty()) {
        builder.Append(selected_text);
    }
}


void SimpleBlockRegion::ChangeFocus(bool is_focused) {
    text_box_->SetIsInFocusContext(is_focused);
}

}
