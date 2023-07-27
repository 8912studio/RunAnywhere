#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/control/text_box.h>
#include "module/command_preview_control.h"

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

    void ChangeStyle(PreviewStyle style);

protected:
    void Layout(const zaf::Rect&) override;

private:
    class LayoutInfo {
    public:
        float required_height{};
        bool need_horizontal_scroll{};
    };

private:
    void AdjustLayout();
    LayoutInfo AdjustTextLayout();
    bool TryToAdjustForSingleLineText();
    bool DeterminateIfAllTextCanFitInSingleLine();
    float GetTextLayoutWidth() const;
    LayoutInfo AdjustForMultiLineText();
    float GetMinTextHeight() const;
    static float CalculateRequriedHeightForMultiLineEdit(
        const zaf::Size& text_preferrence_size,
        std::size_t line_count,
        const zaf::Size& text_bounds,
        std::size_t max_show_line_count);

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::TextBox, textBox);

    PreviewStyle style_{ PreviewStyle::Normal };
    TextDisplayMode display_mode_{ TextDisplayMode::Normal };
    bool has_line_break_{};
};

}