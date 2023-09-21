#pragma once

#include <memory>
#include <vector>
#include "help/markdown/render/render_region.h"
#include "help/markdown/render/style_config.h"

namespace ra::help::markdown::render {

class MarkdownRegion : public RenderRegion {
public:
    static std::unique_ptr<MarkdownRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

public:
    explicit MarkdownRegion(std::vector<std::unique_ptr<RenderRegion>> block_regions);

    void Layout(const zaf::Size& layout_size) override;
    void Paint(zaf::Canvas& canvas) override;

private:
    std::vector<std::unique_ptr<RenderRegion>> block_regions_;
};

}