#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/linear_box.h>
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
    void Layout(const zaf::Rect& previous_rect) override;
    void OnStyleChanged() override;

private:
    void AdjustLayout();
    void AdjustErrorMessageToFitContentSize(bool& is_multiline);
    bool AdjustErrorMessageInSingleLine(float content_width);
    void AdjustErrorMessageInMultilines(float content_width);
    void ChangeLayoutByStyle();

private:
    ZAF_BIND_CONTROL(zaf::HorizontalBox, hexContainer);
    ZAF_BIND_CONTROL(zaf::Label, hexErrorCode);
    ZAF_BIND_CONTROL(utility::PreviewTextBox, errorMessage);
};

}