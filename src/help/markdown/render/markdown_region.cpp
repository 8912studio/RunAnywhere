#include "help/markdown/render/markdown_region.h"
#include <zaf/base/error/check.h>
#include "help/markdown/render/paragraph_region.h"

namespace ra::help::markdown::render {
namespace {

std::unique_ptr<RenderRegion> CreateBlockRegion(const element::Element& element) {

    if (element.Type() == element::ElementType::Paragraph) {
        return ParagraphRegion::Create(element);
    }

    ZAF_NOT_REACHED();
}

}

std::unique_ptr<MarkdownRegion> MarkdownRegion::Create(const element::Element& element) {

    ZAF_EXPECT(element.Type() == element::ElementType::Root);

    std::vector<std::unique_ptr<RenderRegion>> block_regions;

    for (const auto& each_child : element.Children()) {
        auto region = CreateBlockRegion(*each_child);
        block_regions.push_back(std::move(region));
    }

    return std::make_unique<MarkdownRegion>(std::move(block_regions));
}


MarkdownRegion::MarkdownRegion(std::vector<std::unique_ptr<RenderRegion>> block_regions) :
    block_regions_(std::move(block_regions)) {

}


void MarkdownRegion::Resize(const zaf::Size& size) {

    for (const auto& each_region : block_regions_) {
        each_region->Resize(size);
    }
}


void MarkdownRegion::Paint(zaf::Canvas& canvas) {

    for (const auto& each_region : block_regions_) {
        each_region->Paint(canvas);
    }
}

}