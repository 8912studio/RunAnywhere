#pragma once

#include "help/markdown/render/render_region.h"
#include "help/markdown/render/style_config.h"
#include "help/markdown/render/styled_text_layout.h"

namespace ra::help::markdown::render {

class ParagraphRegion : public RenderRegion {
public:
    static std::unique_ptr<ParagraphRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

public:
    explicit ParagraphRegion(StyledTextLayout styled_text_layout);

    void Layout(const zaf::Size& layout_size) override;
    void Paint(zaf::Canvas& canvas) override;

private:
    StyledTextLayout styled_text_layout_;
};

}
