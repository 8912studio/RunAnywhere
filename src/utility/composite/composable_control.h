#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/composite/position_range.h"
#include "utility/composite/selected_text_builder.h"

namespace ra::utility::composite {

class ComposableControl : zaf::NonCopyableNonMovable {
public:
    ComposableControl() = default;
    virtual ~ComposableControl() = default;

    virtual bool IsPositionInsideTextBoundary(const zaf::Point& mouse_position) = 0;

    virtual void BeginSelection(const zaf::Point& position) = 0;
    virtual void ChangeSelection(const PositionRange& position_range) = 0;
    virtual void EndSelection() = 0;

    virtual void SelectWord(const zaf::Point& position) = 0;
    virtual void BuildSelectedText(SelectedTextBuilder& builder) = 0;

    virtual void ChangeFocus(bool is_focused) = 0;
};

}