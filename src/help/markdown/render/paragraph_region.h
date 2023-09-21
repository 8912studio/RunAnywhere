#pragma once

#include <zaf/control/text_box.h>
#include "help/markdown/element/element.h"
#include "help/markdown/render/render_region.h"
#include "help/markdown/render/style_config.h"
#include "help/markdown/render/styled_text.h"

namespace ra::help::markdown::render {

class ParagraphRegion : public RenderRegion {
public:
    static std::shared_ptr<ParagraphRegion> Create(
        const element::Element& element,
        const StyleConfig& style_config);

protected:
    void Initialize() override;
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    ParagraphRegion() = default;

    void SetStyledText(const StyledText& styled_text);

private:
    std::shared_ptr<zaf::TextBox> text_box_;
};

}
