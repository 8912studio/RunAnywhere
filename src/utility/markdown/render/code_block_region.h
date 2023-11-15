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
    void BeginSelection(const zaf::Point& position) override;
    void ChangeSelection(const PositionRange& position_range) override;
    void EndSelection() override;

    bool BuildSelectedText(SelectedTextBuilder& builder) override;

protected:
    void Initialize() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

private:
    CodeBlockRegion() = default;

    void SetStyledText(const std::wstring& text, const TextStyle& text_style);
    void SetTextBackgroundColor(const zaf::Color& color);
    
    bool IsPositionInTextBox(const zaf::Point& position) const;

private:
    std::shared_ptr<utility::ThinScrollControl> scroll_control_;
    std::shared_ptr<StyledTextBox> text_box_;

    std::optional<float> begin_selection_x_offset_;
};

}