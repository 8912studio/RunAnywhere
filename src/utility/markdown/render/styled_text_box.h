#pragma once

#include <zaf/control/text_box.h>
#include "utility/composite/composable_control.h"
#include "utility/markdown/render/position_range.h"
#include "utility/markdown/render/style_config.h"
#include "utility/markdown/render/styled_text.h"

namespace ra::utility::markdown::render {

class StyledTextBox : public zaf::TextBox, public composite::ComposableControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetStyledText(const StyledText& styled_text);

    void SetLineSpacingByParagraphStyle(const StyleConfig& style_config);

    void SetSelectionByPositionRange(
        const PositionRange& position_range, 
        bool scroll_to_selection = false);

    bool IsPositionInsideTextBoundary(const zaf::Point& mouse_position) override;
    void BeginSelection(const zaf::Point& position) override;
    void ChangeSelection(const composite::PositionRange& position_range) override;
    void EndSelection() override;
    void SelectWord(const zaf::Point& position) override;
    void BuildSelectedText(composite::SelectedTextBuilder& builder) override;
    void ChangeFocus(bool is_focused) override;

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

    void SetIsInFocusContext(bool is_focused);

private:
    std::vector<std::pair<zaf::Range, zaf::Color>> background_colors_;
    float line_gap_{};
    bool is_in_focus_context_{};
};

}