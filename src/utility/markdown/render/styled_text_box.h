#pragma once

#include <zaf/control/text_box.h>
#include "utility/markdown/render/position_range.h"
#include "utility/markdown/render/style_config.h"
#include "utility/markdown/render/styled_text.h"

namespace ra::utility::markdown::render {

class StyledTextBox : public zaf::TextBox {
public:
    void SetStyledText(const StyledText& styled_text);

    void SetLineSpacingByParagraphStyle(const StyleConfig& style_config);

    void SetSelectionByPositionRange(
        const PositionRange& position_range, 
        bool scroll_to_selection = false);

protected:
    void Initialize() override;
    void PaintTextBackground(
        zaf::Canvas& canvas,
        const zaf::Rect& dirty_rect, 
        const zaf::TextLayout& text_layout, 
        const zaf::Rect& layout_rect) override;

private:
    void PaintBackgroundColorAtRange(
        zaf::Canvas& canvas, 
        const zaf::TextLayout& text_layout,
        const zaf::Range& range,
        const zaf::Color& color) const;

private:
    std::vector<std::pair<zaf::Range, zaf::Color>> background_colors_;
    float line_gap_{};
};

}