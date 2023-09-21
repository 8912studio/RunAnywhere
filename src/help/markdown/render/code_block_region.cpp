#include "help/markdown/render/code_block_region.h"
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>

namespace ra::help::markdown::render {

std::shared_ptr<CodeBlockRegion> CodeBlockRegion::Create(
    const element::Element& element, 
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::CodeBlock);

    auto result = zaf::Init(new CodeBlockRegion());

    if (!element.Children().empty()) {
        TextStyle text_style;
        text_style.font = style_config.basic_config.font;
        text_style.font.family_name = style_config.code_config.font_family_name;
        text_style.text_color = style_config.code_config.text_color;
        result->SetStyledText(element.Children().front()->Text(), text_style);
    }

    return result;
}


void CodeBlockRegion::Initialize() {

    __super::Initialize();

    SetLayouter(zaf::Create<zaf::VerticalLayouter>());

    text_box_ = zaf::Create<zaf::TextBox>();
    AddChild(text_box_);
}


zaf::Size CodeBlockRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {
    return text_box_->CalculatePreferredSize(bound_size);
}


void CodeBlockRegion::SetStyledText(const std::wstring& text, const TextStyle& text_style) {
    text_box_->SetText(text);
    text_box_->SetFont(text_style.font);
    text_box_->SetTextColor(text_style.text_color);
}

}