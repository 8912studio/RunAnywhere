#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/control/text_box.h>

namespace ra::mod {

enum class TextDisplayMode {
    Normal,
    Base64,
};

class TextContentControl : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetDisplayMode(TextDisplayMode mode);
    void SetText(const std::wstring& text);
    std::wstring GetText() const;

protected:
    void Layout(const zaf::Rect&) override;

    void CalculateAndAdjustControls();
    bool TryToAdjustForSingleLineText();
    bool DeterminateIfAllTextCanFitInSingleLine();
    float GetTextLayoutWidth() const;
    void AdjustForMultiLineText();
    void SetControlHeight(float text_layout_height);
    static float CalculateRequriedHeightForMultiLineEdit(
        const zaf::Size& text_preferrence_size,
        std::size_t line_count,
        const zaf::Size& text_bounds);

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::TextBox, textBox);

    TextDisplayMode display_mode_{ TextDisplayMode::Normal };
    bool has_line_break_{};
};

}