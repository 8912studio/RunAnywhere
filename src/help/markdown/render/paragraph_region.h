#pragma once

#include "help/markdown/element/element.h"
#include "help/markdown/render/render_region.h"
#include "help/markdown/render/style_config.h"
#include "help/markdown/render/styled_text_layout.h"

namespace ra::help::markdown::render {

class ParagraphRegion : public RenderRegion {
public:
    static std::shared_ptr<ParagraphRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

public:
    explicit ParagraphRegion(StyledTextLayout styled_text_layout);

protected:
    void Layout(const zaf::Rect&) override;
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    mutable StyledTextLayout styled_text_layout_;
};

}
