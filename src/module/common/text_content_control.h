#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>

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
    void SetText(std::wstring text);

protected:
    void Layout(const zaf::Rect&) override;

    void CalculateAndAdjustControls();
    bool TryToAdjustForSingleLineText();
    bool DeterminateIfAllTextCanFitInSingleLine(bool& has_set_text);
    float GetTextLayoutWidth() const;
    void BreakTextForBase64Mode();
    void AdjustForMultiLineText(bool has_set_text);
    static float CalculateRequriedHeightForMultiLineEdit(
        const zaf::Size& text_preferrence_size,
        std::size_t line_count,
        const zaf::Size& text_bounds);

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, richEdit);

    TextDisplayMode display_mode_{ TextDisplayMode::Normal };
    std::wstring text_;
    bool has_line_break_{};
};

}