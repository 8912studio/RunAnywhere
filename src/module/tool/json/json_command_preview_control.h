#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/scroll_box.h>
#include <zaf/control/text_box.h>
#include "module/command_preview_control.h"
#include "module/common/error_view.h"
#include "module/tool/json/json_command_parse_result.h"

namespace ra::mod::tool::json {

class JSONCommandPreviewControl : public CommandPreviewControl {
public:
    ZAF_OBJECT;

    //For unit test.
    class ErrorLineInfo {
    public:
        std::vector<std::wstring> lines;
        std::size_t error_line_index{};
        std::size_t error_char_index{};
    };
    static ErrorLineInfo GetAdjacentLinesAtErrorIndex(std::wstring_view text, std::size_t index);

    class ErrorContent {
    public:
        std::wstring shown_text;
        std::size_t error_index{};
    };
    static ErrorContent GetShownErrorContent(
        const ErrorLineInfo& error_info, 
        std::size_t max_char_count);

    static zaf::textual::StyledText GenerateParseErrorText(
        const ErrorContent& error_info,
        const zaf::Font& font);

public:
    void ShowResult(const JSONCommandParseResult& result);

protected:
    void Layout(const zaf::Rect&) override;
    void OnStyleChanged() override;
    zaf::Frame GetExpectedMargin() override;
    
private:
    void ShowParseError(const JSONCommandParseResult::ErrorInfo& error);
    void ShowGenericError(const JSONCommandParseResult::ErrorInfo& error);
    void ResetParsedJSON();
    void ResetParseError();
    void ResetScrollControlHeight();
    float BodyFontSize() const;

private:
    ZAF_BIND_CONTROL(zaf::Control, contentView);
    ZAF_BIND_CONTROL(zaf::Control, parseErrorView);
    ZAF_BIND_CONTROL(zaf::Label, parseErrorLabel);
    ZAF_BIND_CONTROL(zaf::ScrollBox, scrollControl);
    ZAF_BIND_CONTROL(zaf::TextBox, textBox);
    ZAF_BIND_CONTROL(ErrorView, genericErrorView);

    zaf::textual::StyledText parsed_json_;
    std::optional<ErrorLineInfo> error_line_info_;
};

ZAF_OBJECT_BEGIN(JSONCommandPreviewControl)
ZAF_OBJECT_RESOURCE_URI(L"res:///module/tool/json/json_command_preview_control.xaml")
ZAF_OBJECT_END;

}