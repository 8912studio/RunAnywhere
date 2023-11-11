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


void SimpleBlockRegion::ChangeSelection(
    const zaf::Point& begin_position,
    const zaf::Point& end_position) {

    text_box_->SetSelectionByPositionRange(
        this->TranslatePositionToChild(begin_position, *text_box_),
        this->TranslatePositionToChild(end_position, *text_box_));
}

}
