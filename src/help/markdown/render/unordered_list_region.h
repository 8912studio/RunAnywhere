#pragma once

#include "help/markdown/element/element.h"
#include "help/markdown/render/render_region.h"
#include "help/markdown/render/style_config.h"

namespace ra::help::markdown::render {

class UnorderedListRegion : public RenderRegion {
public:
    static std::shared_ptr<UnorderedListRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

protected:


private:
    UnorderedListRegion() = default;
};

}