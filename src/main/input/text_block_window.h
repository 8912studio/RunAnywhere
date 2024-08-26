#pragma once

#include <zaf/base/com_object.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/scroll_box.h>
#include <zaf/control/text_box.h>
#include "main/input/argument_object_window.h"
#include "main/input/line_break_option.h"
#include "utility/text_utility.h"

namespace ra::main::input {

class TextBlockWindow : public ArgumentObjectWindow {
public:
    ZAF_OBJECT;

public:
    TextBlockWindow();

    void SetIsReadOnly(bool read_only) override;

    std::wstring GetText() override;
    void SetText(const std::wstring& text) override;

protected:
    void AfterParse() override;
    void OnShow(const zaf::ShowInfo& event_info) override;

    zaf::Size CalculateWindowContentSize() override;

private:
    void UpdateLineBreakOptions(const utility::LineBreakInfo& line_break_info);
    utility::LineBreak GetLineBreakByOption(const LineBreakOption& option) const;
    void OnLineBreakOptionClick(const zaf::MouseUpInfo& event_info);

private:
    ZAF_BIND_CONTROL(zaf::Control, lineBreakOptions);
    ZAF_BIND_CONTROL(LineBreakOption, useCRLF);
    ZAF_BIND_CONTROL(LineBreakOption, useCR);
    ZAF_BIND_CONTROL(LineBreakOption, useLF);
    ZAF_BIND_CONTROL(zaf::ScrollBox, scrollableControl);
    ZAF_BIND_CONTROL(zaf::TextBox, textEdit);

    bool is_setting_text_{};
};

ZAF_OBJECT_BEGIN(TextBlockWindow)
ZAF_OBJECT_RESOURCE_URI(L"res:///main/input/text_block_window.xaml")
ZAF_OBJECT_END;

}

