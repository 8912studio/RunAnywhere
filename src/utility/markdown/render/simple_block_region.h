#pragma once

#include <zaf/control/text_box.h>
#include "utility/composite/composable_control.h"
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/render_region.h"
#include "utility/markdown/render/style_config.h"
#include "utility/markdown/render/styled_text.h"
#include "utility/markdown/render/styled_text_box.h"

namespace ra::utility::markdown::render {

class SimpleBlockRegion : public RenderRegion {
public:
    SimpleBlockRegion() = default;

    void InitializeContent(const element::Element& element, const StyleConfig& style_config);

protected:
    void Initialize() override;
    void Paint(zaf::Canvas& canvas, const zaf::Rect& dirty_rect) override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

    const std::shared_ptr<StyledTextBox>& TextBox() const {
        return text_box_;
    }

private:
    std::shared_ptr<StyledTextBox> text_box_;
};

}
