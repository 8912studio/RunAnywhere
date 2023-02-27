#pragma once

#include <zaf/control/control_binder.h>
#include "module/command_preview_control.h"
#include "module/tool/error/error_command_parse_result.h"
#include "utility/preview_text_box.h"

namespace ra::module::tool::error {

class ErrorPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

public:
    explicit ErrorPreviewControl(const std::optional<ErrorCommandParseResult>& parse_result);

    std::wstring GetText() const;

protected:
    void AfterParse() override;

private:
    void ShowResult() const;

private:
    ZAF_BIND_CONTROL(utility::PreviewTextBox, errorCode);
    ZAF_BIND_CONTROL(utility::PreviewTextBox, errorMessage);

    std::optional<ErrorCommandParseResult> parse_result_;
};

}