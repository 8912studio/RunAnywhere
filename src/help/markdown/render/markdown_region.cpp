#include "help/markdown/render/markdown_region.h"
#include <zaf/base/error/check.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>

namespace ra::help::markdown::render {

std::shared_ptr<MarkdownRegion> MarkdownRegion::Create(
    const element::Element& element,
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Root);

    auto body_region = BodyRegion::Create(element.Children(), style_config, 0);
    return zaf::Init(new MarkdownRegion(body_region));
}


MarkdownRegion::MarkdownRegion(std::shared_ptr<BodyRegion> body_region) :
    body_region_(std::move(body_region)) {

}


void MarkdownRegion::Initialize() {

    __super::Initialize();

    this->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    this->SetPadding(zaf::Frame{ 12, 12, 2, 12 });

    AddChild(body_region_);
}


zaf::Size MarkdownRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {
    return body_region_->CalculatePreferredSize(bound_size);
}

}