#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"

namespace ra::mod {

class TextPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetText(const std::wstring& text);
    void SetWrapText(bool wrap);

protected:
    zaf::Frame GetExpectedMargin() override;
    void Layout(const zaf::Rect&) override;

private:
    void ShowEmptyText();
    void ShowText(const std::wstring& text);

    void CalculateAndAdjustControls();
    bool TryToAdjustForSingleLineText();
    void AdjustForMultiLineText();
    static float CalculateRequriedHeightForMultiLineEdit(
        const zaf::Size& edit_size,
        std::size_t line_count,
        const zaf::Size& bounds,
        bool& need_horizontal_scroll, 
        bool& need_vertical_scroll);

private:
    ZAF_BIND_CONTROL(zaf::Label, emptyLabel);
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, richEdit);

    bool wrap_text_{};
    bool has_line_break_{};
};

}