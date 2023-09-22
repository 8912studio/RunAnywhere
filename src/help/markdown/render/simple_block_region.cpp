#include "help/markdown/render/simple_block_region.h"
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include <zaf/graphic/canvas.h>
#include "help/markdown/render/styled_text_builder.h"

namespace ra::help::markdown::render {

void SimpleBlockRegion::Initialize() {

    __super::Initialize();

    SetLayouter(zaf::Create<zaf::VerticalLayouter>());

    text_box_ = zaf::Create<zaf::TextBox>();
    text_box_->SetIsEnabled(false);
    text_box_->SetWordWrapping(zaf::WordWrapping::Wrap);
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

    text_box_->SetText(styled_text.Text());

    for (const auto& each_style : styled_text.Styles()) {
        text_box_->SetFontAtRange(each_style.style.font, each_style.range);
        text_box_->SetTextColorAtRange(each_style.style.text_color, each_style.range);
    }
}

}
