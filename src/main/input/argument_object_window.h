#pragma once

#include <zaf/base/com_object.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/rx/subject.h>
#include "main/input/line_break_option.h"
#include "utility/text_utility.h"
#include "utility/thin_border_window.h"

namespace ra::main::input {

class ArgumentObjectWindow : public utility::ThinBorderWindow {
public:
    ZAF_DECLARE_TYPE;

public:
    ArgumentObjectWindow();

    void SetObjectPositionInScreen(const zaf::Point& position);

    void SetHeaderTitle(const std::wstring& title, const zaf::Color& text_color);
    void SetIsReadOnly(bool read_only);
    void SetIsMultiline(bool multiline);

    std::wstring GetText() const;
    void SetText(const std::wstring& text);

    zaf::Observable<std::shared_ptr<ArgumentObjectWindow>> TextChangedEvent();

protected:
    void AfterParse() override;
    void OnDeactivated(const zaf::DeactivatedInfo& event_info) override;
    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;
    void OnShow(const zaf::ShowInfo& event_info) override;

private:
    void AdjustPositionAndSize();
    void UpdateLineBreakOptions();
    utility::LineBreak GetLineBreakByOption(const LineBreakOption& option) const;
    void OnLineBreakOptionClick(const zaf::MouseUpInfo& event_info);
    void RaiseTextChangedEvent();

private:
    ZAF_BIND_CONTROL(zaf::Control, header);
    ZAF_BIND_CONTROL(zaf::Label, titleLabel);
    ZAF_BIND_CONTROL(zaf::Control, lineBreakOptions);
    ZAF_BIND_CONTROL(LineBreakOption, useCRLF);
    ZAF_BIND_CONTROL(LineBreakOption, useCR);
    ZAF_BIND_CONTROL(LineBreakOption, useLF);
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollableControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, textEdit);

    zaf::Point object_position_in_screen_;
    zaf::Subject<std::shared_ptr<ArgumentObjectWindow>> text_changed_event_;

    utility::LineBreakInfo line_break_info_;
};

}

