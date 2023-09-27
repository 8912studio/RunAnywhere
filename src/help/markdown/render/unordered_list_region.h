#pragma once

#include "help/markdown/element/element.h"
#include "help/markdown/render/compound_region.h"
#include "help/markdown/render/style_config.h"

namespace ra::help::markdown::render {

class UnorderedListRegion : public CompoundRegion {
public:
    static std::shared_ptr<UnorderedListRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

private:
    explicit UnorderedListRegion(std::vector<std::shared_ptr<RenderRegion>> item_regions);
};

}