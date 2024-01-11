#pragma once

#include "utility/composite/composite_control.h"
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/render_region.h"
#include "utility/markdown/render/style_config.h"

namespace ra::utility::markdown::render {

class CompoundRegion : public RenderRegion, public composite::CompositeControl {
protected:
    explicit CompoundRegion(std::vector<std::shared_ptr<RenderRegion>> child_regions);

    void Initialize() override;
    void Layout(const zaf::Rect&) override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    std::vector<std::shared_ptr<RenderRegion>> child_regions_;
};

}