#include "help/markdown/render/markdown_region.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>
#include "help/markdown/render/code_block_region.h"
#include "help/markdown/render/simple_block_region.h"

namespace ra::help::markdown::render {

std::shared_ptr<MarkdownRegion> MarkdownRegion::Create(
    const element::Element& element,
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Root);

    std::vector<std::shared_ptr<RenderRegion>> block_regions;

    for (const auto& each_child : element.Children()) {
        auto region = CreateBlockRegion(*each_child, style_config);
        block_regions.push_back(std::move(region));
    }

    return zaf::Create<MarkdownRegion>(std::move(block_regions));
}


std::shared_ptr<RenderRegion> MarkdownRegion::CreateBlockRegion(
    const element::Element& element, 
    const StyleConfig& style_config) {

    switch (element.Type()) {
    case element::ElementType::Paragraph:
    case element::ElementType::Header: {
        auto region = zaf::Create<SimpleBlockRegion>();
        region->InitializeContent(element, style_config);
        return region;
    }
    case element::ElementType::CodeBlock:
        return CodeBlockRegion::Create(element, style_config);
    default:
        ZAF_NOT_REACHED();
    }
}


MarkdownRegion::MarkdownRegion(std::vector<std::shared_ptr<RenderRegion>> block_regions) :
    block_regions_(std::move(block_regions)) {

}


void MarkdownRegion::Initialize() {

    __super::Initialize();

    std::vector<std::shared_ptr<zaf::Control>> children;
    for (const auto& each_region : block_regions_) {
        children.push_back(each_region);
    }
    AddChildren(children);
}


void MarkdownRegion::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    zaf::Size bound_size{};
    bound_size.width = this->Width();
    bound_size.height = std::numeric_limits<float>::max();

    float region_y{};
    for (const auto& each_region : block_regions_) {

        zaf::Rect region_rect;
        region_rect.position.x = 0;
        region_rect.position.y = region_y;
        region_rect.size = each_region->CalculatePreferredSize(bound_size);
        each_region->SetRect(region_rect);

        region_y += region_rect.size.height;
    }
}


zaf::Size MarkdownRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    zaf::Size result;
    result.width = bound_size.width;

    zaf::Size child_bound_size{};
    child_bound_size.width = bound_size.width;
    child_bound_size.height = std::numeric_limits<float>::max();
    
    for (const auto& each_region : block_regions_) {

        auto child_size = each_region->CalculatePreferredSize(child_bound_size);
        result.height += child_size.height;
    }

    return result;
}

}