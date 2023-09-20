#pragma once

#include <memory>
#include <vector>
#include "help/markdown/render/render_region.h"

namespace ra::help::markdown::render {

class MarkdownRegion : public RenderRegion {
public:
    static std::unique_ptr<MarkdownRegion> Create(const element::Element& element);

public:
    explicit MarkdownRegion(std::vector<std::unique_ptr<RenderRegion>> block_regions);

    void Resize(const zaf::Size& size) override;
    void Paint(zaf::Canvas& canvas) override;

private:
    std::vector<std::unique_ptr<RenderRegion>> block_regions_;
};

}