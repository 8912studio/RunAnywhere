#pragma once

#include <zaf/graphic/text/text_layout.h>
#include "help/markdown/render/render_region.h"
#include "help/markdown/render/styled_text.h"

namespace ra::help::markdown::render {

class ParagraphRegion : public RenderRegion {
public:
    explicit ParagraphRegion(const StyledText& styled_text);

    void Resize(const zaf::Size& size) override;
    void Paint(zaf::Canvas& canvas) override;

private:
    zaf::TextLayout text_layout_;
};

}
