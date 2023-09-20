#pragma once

#include "help/markdown/render/render_region.h"

namespace ra::help::markdown::render {

class MarkdownRegion : public RenderRegion {
public:
    void Resize(const zaf::Size& size) {}
    void Paint(zaf::Canvas& canvas) {}
};

}