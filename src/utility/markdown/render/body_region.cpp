#include "utility/markdown/render/body_region.h"
#include <zaf/base/as.h>
#include <zaf/creation.h>
#include "utility/markdown/element/header_element.h"
#include "utility/markdown/render/code_block_region.h"
#include "utility/markdown/render/header_region.h"
#include "utility/markdown/render/paragraph_region.h"
#include "utility/markdown/render/unordered_list_region.h"

namespace ra::utility::markdown::render {

std::shared_ptr<BodyRegion> BodyRegion::Create(
    const element::ElementList& block_elements, 
    const StyleConfig& style_config,
    std::size_t depth) {

    std::vector<std::shared_ptr<RenderRegion>> block_regions;
    for (auto index : zaf::Range(0, block_elements.size())) {

        const auto& element = block_elements[index];
        auto region = CreateBlockRegion(*element, style_config, depth);

        auto position =
            index == 0 ? BlockPosition::Start :
            index == block_elements.size() - 1 ? BlockPosition::End :
            BlockPosition::Middle;

        auto margin = GetBlockMargin(*element, position, style_config);
        region->SetMargin(margin);

        block_regions.push_back(std::move(region));
    }

    return zaf::Init(new BodyRegion(std::move(block_regions)));
}


std::shared_ptr<RenderRegion> BodyRegion::CreateBlockRegion(
    const element::Element& element, 
    const StyleConfig& style_config,
    std::size_t depth) {

    switch (element.Type()) {
    case element::ElementType::Paragraph:
        return ParagraphRegion::Create(element, style_config);
    case element::ElementType::Header:
        return HeaderRegion::Create(element, style_config);
    case element::ElementType::CodeBlock:
        return CodeBlockRegion::Create(element, style_config);
    case element::ElementType::UnorderedList:
        return UnorderedListRegion::Create(element, style_config, depth);
    default:
        ZAF_NOT_REACHED();
    }
}


zaf::Frame BodyRegion::GetBlockMargin(
    const element::Element& element,
    BlockPosition position,
    const StyleConfig& style_config) {

    zaf::Frame result;

    if (position != BlockPosition::Start) {

        result.top = style_config.block_gap;

        if (element.Type() == element::ElementType::Paragraph || 
            element.Type() == element::ElementType::UnorderedList) {
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


BodyRegion::BodyRegion(std::vector<std::shared_ptr<RenderRegion>> block_regions) : 
    CompoundRegion(std::move(block_regions)) {

}

}