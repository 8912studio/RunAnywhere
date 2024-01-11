#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
#include "utility/composite/composable_control.h"
#include "utility/markdown/element/element.h"
#include "utility/markdown/render/render_region.h"
#include "utility/markdown/render/style_config.h"
#include "utility/markdown/render/styled_text_box.h"
#include "utility/markdown/render/text_style.h"
#include "utility/thin_scroll_control.h"

namespace ra::utility::markdown::render {

class CodeBlockRegion : public RenderRegion, public composite::ComposableControl {
public:
    ZAF_DECLARE_TYPE;

    static std::shared_ptr<CodeBlockRegion> Create(
        const element::Element& element, 
        const StyleConfig& style_config);

public:
    bool IsPositionInsideTextBoundary(const zaf::Point& mouse_position) override;
    void BeginSelection(const zaf::Point& position) override;
    void ChangeSelection(const composite::PositionRange& position_range) override;
    void EndSelection() override;

    void SelectWord(const zaf::Point& position) override;

    void BuildSelectedText(composite::SelectedTextBuilder& builder) override;

    void ChangeFocus(bool is_focused) override;

protected:
    void AfterParse() override;
    zaf::Size CalculatePreferredContentSize(const zaf::Size& bound_size) const override;

    void OnMouseEnter(const zaf::MouseEnterInfo&) override;
    void OnMouseLeave(const zaf::MouseLeaveInfo&) override;

private:
    CodeBlockRegion() = default;

    void SetStyledText(const std::wstring& text, const TextStyle& text_style);
    void SetShowHeader(bool show);
    void SetLanguage(const std::wstring& language);
    void SetTextBackgroundColor(const zaf::Color& color);
    
    bool IsPositionInTextBox(const zaf::Point& position) const;

private:
    ZAF_BIND_CONTROL(zaf::Control, header);
    ZAF_BIND_CONTROL(zaf::Label, languageLabel);
    ZAF_BIND_CONTROL(zaf::Button, copyButton);
    ZAF_BIND_CONTROL(utility::ThinScrollControl, scrollControl);
    ZAF_BIND_CONTROL(StyledTextBox, textBox);

    std::optional<float> begin_selection_x_offset_;
};

}