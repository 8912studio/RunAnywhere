#pragma once

#include "help/markdown/render/render_region.h"
#include "help/markdown/render/styled_text_layout.h"

namespace ra::help::markdown::render {

class ParagraphRegion : public RenderRegion {
public:
    static std::unique_ptr<ParagraphRegion> Create(const element::Element& element);

public:
    explicit ParagraphRegion(StyledTextLayout styled_text_layout);

    void Resize(const zaf::Size& size) override;
    void Paint(zaf::Canvas& canvas) override;

private:
    StyledTextLayout styled_text_layout_;
};

}
