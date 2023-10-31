#include "utility/markdown/render/code_block_region.h"
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

    text_box_ = zaf::Create<zaf::TextBox>();
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

}