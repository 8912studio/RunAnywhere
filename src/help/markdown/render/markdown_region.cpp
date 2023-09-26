#include "help/markdown/render/markdown_region.h"
#include <zaf/base/as.h>
#include <zaf/base/error/check.h>
#include <zaf/base/range.h>
#include <zaf/creation.h>
#include "help/markdown/element/header_element.h"
#include "help/markdown/render/code_block_region.h"
#include "help/markdown/render/paragraph_region.h"
#include "help/markdown/render/simple_block_region.h"

namespace ra::help::markdown::render {

std::shared_ptr<MarkdownRegion> MarkdownRegion::Create(
    const element::Element& element,
    const StyleConfig& style_config) {

    ZAF_EXPECT(element.Type() == element::ElementType::Root);

    std::vector<std::shared_ptr<RenderRegion>> block_regions;
    const auto& block_elements = element.Children();
    for (auto index : zaf::Range(0, block_elements.size())) {

        const auto& element = block_elements[index];
        auto region = CreateBlockRegion(*element, style_config);

        auto position =
            index == 0 ? BlockPosition::Start :
            index == block_elements.size() - 1 ? BlockPosition::End :
            BlockPosition::Middle;

        auto margin = GetBlockMargin(*element, position, style_config);
        region->SetMargin(margin);

        block_regions.push_back(std::move(region));
    }

    return zaf::Create<MarkdownRegion>(std::move(block_regions));
}


std::shared_ptr<RenderRegion> MarkdownRegion::CreateBlockRegion(
    const element::Element& element, 
    const StyleConfig& style_config) {

    switch (element.Type()) {
    case element::ElementType::Paragraph: 
        return ParagraphRegion::Create(element, style_config);
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


zaf::Frame MarkdownRegion::GetBlockMargin(
    const element::Element& element,
    BlockPosition position,
    const StyleConfig& style_config) {

    zaf::Frame result;

    if (position != BlockPosition::Start) {

        result.top = style_config.block_gap;

        if (element.Type() == element::ElementType::Paragraph) {
            //Line gap addes extra spacing before paragraph, making block gap be larger than 
            //expected, so we need to substrct line gap from block gap.
            result.top -= style_config.paragraph_config.line_gap;
        }
        else if (auto header_element = zaf::As<element::HeaderElement>(&element)) {
            result.top += style_config.GetHeaderConfig(header_element->Depth()).top_spacing;
        }
    }

    return result;
}


MarkdownRegion::MarkdownRegion(std::vector<std::shared_ptr<RenderRegion>> block_regions) :
    block_regions_(std::move(block_regions)) {

}


void MarkdownRegion::Initialize() {

    __super::Initialize();

    this->SetPadding(zaf::Frame{ 12, 12, 2, 12 });

    std::vector<std::shared_ptr<zaf::Control>> children;
    for (const auto& each_region : block_regions_) {
        children.push_back(each_region);
    }
    AddChildren(children);
}


void MarkdownRegion::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    zaf::Size bound_size{};
    bound_size.width = this->ContentSize().width;
    bound_size.height = std::numeric_limits<float>::max();

    float region_y{};
    for (const auto& each_region : block_regions_) {

        region_y += each_region->Margin().top;

        zaf::Rect region_rect;
        region_rect.position.x = 0;
        region_rect.position.y = region_y;
        region_rect.size.width = bound_size.width;
        region_rect.size.height = each_region->CalculatePreferredSize(bound_size).height;
        each_region->SetRect(region_rect);

        region_y += region_rect.size.height + each_region->Margin().bottom;
    }
}


zaf::Size MarkdownRegion::CalculatePreferredContentSize(const zaf::Size& bound_size) const {

    zaf::Size result;
    result.width = bound_size.width - this->Border().Width() - this->Padding().Width();
    if (result.width <= 0) {
        return {};
    }

    zaf::Size child_bound_size{};
    child_bound_size.width = result.width;
    child_bound_size.height = std::numeric_limits<float>::max();
    
    for (const auto& each_region : block_regions_) {

        auto child_size = each_region->CalculatePreferredSize(child_bound_size);
        result.height += child_size.height + each_region->Margin().Height();
    }

    return result;
}

}