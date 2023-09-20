#pragma once

#include <memory>
#include "help/markdown/element/element.h"
#include "help/markdown/render/markdown_region.h"

namespace ra::help::markdown::render {

class MarkdownRegionBuilder {
public:
    std::unique_ptr<MarkdownRegion> Build(const element::Element& root_elment);
};

}