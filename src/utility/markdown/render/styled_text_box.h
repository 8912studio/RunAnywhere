#pragma once

#include <zaf/control/text_box.h>
#include "utility/composite/composable_control.h"
#include "utility/composite/position_range.h"
#include "utility/markdown/render/style_config.h"

namespace ra::utility::markdown::render {

class StyledTextBox : public zaf::TextBox, public composite::ComposableControl {
public:
    ZAF_OBJECT;

public:
    void SetLineSpacingByParagraphStyle(const StyleConfig& style_config);

    void SetSelectionByPositionRange(
        const composite::PositionRange& position_range, 
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

private:
    void SetIsInFocusContext(bool is_focused);

private:
    bool is_in_focus_context_{};
};

ZAF_OBJECT_BEGIN(StyledTextBox)
ZAF_OBJECT_END;

}