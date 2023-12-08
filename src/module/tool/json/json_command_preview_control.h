#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/scrollable_control.h>
#include "module/command_preview_control.h"
#include "module/common/error_view.h"
#include "module/tool/json/json_command_parse_result.h"
#include "utility/markdown/render/styled_text_box.h"

namespace ra::mod::tool::json {

class JSONCommandPreviewControl : public CommandPreviewControl {
public:
    ZAF_DECLARE_TYPE;

    //For unit test.
    class ErrorLineInfo {
    public:
        std::vector<std::wstring> lines;
        std::size_t error_line_index{};
        std::size_t error_char_index{};
    };
    static ErrorLineInfo GetAdjacentLinesAtErrorIndex(std::wstring_view text, std::size_t index);
    static utility::markdown::render::StyledText GenerateParseErrorText(
        const ErrorLineInfo& error_info);

public:
    void ShowResult(const JSONCommandParseResult& result);

protected:
    void Layout(const zaf::Rect&) override;
    void OnStyleChanged() override;
    zaf::Frame GetExpectedMargin() override;
    
private:
    void ShowParseError(const JSONCommandParseResult::ErrorInfo& error);
    void ShowGenericError(const JSONCommandParseResult::ErrorInfo& error);
    void ResetScrollControlHeight();

private:
    ZAF_BIND_CONTROL(zaf::Control, contentView);
    ZAF_BIND_CONTROL(zaf::Control, parseErrorView);
    ZAF_BIND_CONTROL(zaf::Label, parseErrorLabel);
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollControl);
    ZAF_BIND_CONTROL(utility::markdown::render::StyledTextBox, textBox);
    ZAF_BIND_CONTROL(ErrorView, genericErrorView);
};

}