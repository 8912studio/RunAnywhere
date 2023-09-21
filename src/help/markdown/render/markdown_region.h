#pragma once

#include <memory>
#include <vector>
#include "help/markdown/element/element.h"
#include "help/markdown/render/render_region.h"
#include "help/markdown/render/style_config.h"

namespace ra::help::markdown::render {

class MarkdownRegion : public RenderRegion {
public:
    static std::shared_ptr<MarkdownRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

public:
    explicit MarkdownRegion(std::vector<std::shared_ptr<RenderRegion>> block_regions);

protected:
    void Initialize() override;
    void Layout(const zaf::Rect& previous_rect) override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    std::vector<std::shared_ptr<RenderRegion>> block_regions_;
};

}