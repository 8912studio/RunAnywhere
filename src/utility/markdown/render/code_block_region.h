#pragma once

#include "utility/markdown/element/element.h"
#include "utility/markdown/render/render_region.h"
#include "utility/markdown/render/style_config.h"
#include "utility/markdown/render/styled_text_box.h"
#include "utility/markdown/render/text_style.h"
#include "utility/thin_scroll_control.h"

namespace ra::utility::markdown::render {

class CodeBlockRegion : public RenderRegion {
public:
    static std::shared_ptr<CodeBlockRegion> Create(
        const element::Element& element, 
        const StyleConfig& style_config);

public:
    void ChangeSelection(
        const zaf::Point& begin_position,
        const zaf::Point& end_position) override;

protected:
    void Initialize() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    CodeBlockRegion() = default;

    void SetStyledText(const std::wstring& text, const TextStyle& text_style);
    void SetTextBackgroundColor(const zaf::Color& color);

private:
    std::shared_ptr<utility::ThinScrollControl> scroll_control_;
    std::shared_ptr<StyledTextBox> text_box_;
};

}