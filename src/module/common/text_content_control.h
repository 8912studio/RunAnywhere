#pragma once

#include <zaf/control/control.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/control/text_box.h>
#include "module/command_preview_control.h"

namespace ra::mod {

class TextDisplayMode {
public:
    bool use_fixed_width_font{};
    zaf::WordWrapping word_wrapping{ zaf::WordWrapping::NoWrap };
};

class TextContentControl : public zaf::Control {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetDisplayMode(const TextDisplayMode& mode);
    void SetText(const std::wstring& text);
    std::wstring GetText() const;

    void ChangeStyle(CommandDisplayStyle style);

protected:
    void AfterParse() override;
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

    CommandDisplayStyle style_{ CommandDisplayStyle::Normal };
    TextDisplayMode display_mode_;
    bool has_line_break_{};
};

}