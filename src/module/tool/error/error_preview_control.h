#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include "module/command_preview_control.h"
#include "module/tool/error/error_command_parse_result.h"
#include "utility/preview_text_box.h"

namespace ra::mod::tool::error {

class ErrorPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    void ShowErrorMessage(const ErrorCommandParseResult& parse_result);

    std::wstring GetText() const;

protected:
    void OnRectChanged(const zaf::RectChangedInfo& event_info) override;
    void OnStyleChanged() override;

private:
    void AdjustErrorMessageToFitContentSize();
    zaf::TextLayout CreateTextLayoutForMeasuring() const;
    bool AdjustErrorMessageByReducingFontSize(zaf::TextLayout& text_layout, float content_width);
    void AdjustErrorMessageByAddingLines(zaf::TextLayout& text_layout, float content_width);

private:
    ZAF_BIND_CONTROL(zaf::Control, resultView);
    ZAF_BIND_CONTROL(zaf::Label, hexErrorCode);
    ZAF_BIND_CONTROL(utility::PreviewTextBox, errorMessage);
};

}