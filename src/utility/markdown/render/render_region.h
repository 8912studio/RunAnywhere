#pragma once

#include <zaf/control/control.h>
#include "utility/markdown/render/position_range.h"
#include "utility/markdown/render/selected_text_builder.h"

namespace ra::utility::markdown::render {

class RenderRegion : public zaf::Control {
public:
    virtual void BeginSelection(const zaf::Point& position) { }

    virtual void ChangeSelection(const PositionRange& position_range) = 0;

    virtual void EndSelection() { }

    virtual void BuildSelectedText(SelectedTextBuilder& builder) = 0;
};

}