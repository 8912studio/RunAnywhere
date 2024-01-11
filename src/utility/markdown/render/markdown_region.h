#pragma once

#include <memory>
#include <vector>
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/body_region.h"

namespace ra::utility::markdown::render {

class MarkdownRegion : public RenderRegion {
public:
    static std::shared_ptr<MarkdownRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

protected:
    void Initialize() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    explicit MarkdownRegion(std::shared_ptr<BodyRegion> body_region);

private:
    std::shared_ptr<BodyRegion> body_region_;
};

}