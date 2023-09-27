#pragma once

#include "help/markdown/element/element.h"
#include "help/markdown/render/compound_region.h"
#include "help/markdown/render/style_config.h"

namespace ra::help::markdown::render {

class BodyRegion : public CompoundRegion {
public:
    static std::shared_ptr<BodyRegion> Create(
        const element::ElementList& block_elements, 
        const StyleConfig& style_config);

private:
    enum class BlockPosition {
        Start,
        Middle,
        End,
    };

    static std::shared_ptr<RenderRegion> CreateBlockRegion(
        const element::Element& element,
        const StyleConfig& style_config);

    static zaf::Frame GetBlockMargin(
        const element::Element& element,
        BlockPosition position,
        const StyleConfig& style_config);

private:
    explicit BodyRegion(std::vector<std::shared_ptr<RenderRegion>> block_regions);
};

}