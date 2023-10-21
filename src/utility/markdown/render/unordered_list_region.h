#pragma once

#include "utility/markdown/element/element.h"
#include "utility/markdown/render/compound_region.h"
#include "utility/markdown/render/style_config.h"

namespace ra::utility::markdown::render {

class UnorderedListRegion : public CompoundRegion {
public:
    static std::shared_ptr<UnorderedListRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config,
        std::size_t depth);

private:
    explicit UnorderedListRegion(std::vector<std::shared_ptr<RenderRegion>> item_regions);
};

}