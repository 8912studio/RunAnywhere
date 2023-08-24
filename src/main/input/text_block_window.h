#pragma once

#include <zaf/base/com_object.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include "main/input/argument_object_window.h"
#include "main/input/line_break_option.h"
#include "utility/text_utility.h"

namespace ra::main::input {

class TextBlockWindow : public ArgumentObjectWindow {
public:
    ZAF_DECLARE_TYPE;

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
    void UpdateLineBreakOptions();
    utility::LineBreak GetLineBreakByOption(const LineBreakOption& option) const;
    void OnLineBreakOptionClick(const zaf::MouseUpInfo& event_info);

private:
    ZAF_BIND_CONTROL(zaf::Control, lineBreakOptions);
    ZAF_BIND_CONTROL(LineBreakOption, useCRLF);
    ZAF_BIND_CONTROL(LineBreakOption, useCR);
    ZAF_BIND_CONTROL(LineBreakOption, useLF);
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollableControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, textEdit);

    utility::LineBreakInfo line_break_info_;
};

}

