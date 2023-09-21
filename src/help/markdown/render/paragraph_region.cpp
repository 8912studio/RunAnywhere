#include "help/markdown/render/paragraph_region.h"
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include <zaf/graphic/canvas.h>
#include "help/markdown/render/styled_text_builder.h"

namespace ra::help::markdown::render {

std::shared_ptr<ParagraphRegion> ParagraphRegion::Create(
    const element::Element& element,
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Paragraph);

    StyledTextBuilder styled_text_builder;
    auto styled_text = styled_text_builder.Build(element, style_config);

    auto result = zaf::Init(new ParagraphRegion());
    result->SetStyledText(styled_text);
    return result;
}


void ParagraphRegion::Initialize() {

    __super::Initialize();

    SetLayouter(zaf::Create<zaf::VerticalLayouter>());

    text_box_ = zaf::Create<zaf::TextBox>();
    AddChild(text_box_);
}


void ParagraphRegion::Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) {

    __super::Paint(canvas, dirty_rect);
}


zaf::Size ParagraphRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {
    return text_box_->CalculatePreferredSize(bound_size);
}


void ParagraphRegion::SetStyledText(const StyledText& styled_text) {

    text_box_->SetText(styled_text.Text());

    for (const auto& each_style : styled_text.Styles()) {
        text_box_->SetFontAtRange(each_style.style.font, each_style.range);
        text_box_->SetTextColorAtRange(each_style.style.text_color, each_style.range);
    }
}

}
