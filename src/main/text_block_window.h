#pragma once

#include <zaf/base/com_object.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/scrollable_control.h>
#include <zaf/rx/subject.h>
#include "utility/thin_border_window.h"

namespace ra {

class TextBlockWindow : public utility::ThinBorderWindow {
public:
    ZAF_DECLARE_TYPE;

public:
    TextBlockWindow();

    std::wstring GetText() const;
    void SetText(const std::wstring& text);

    zaf::Observable<std::shared_ptr<TextBlockWindow>> TextChangedEvent();

protected:
    void AfterParse() override;
    void OnDeactivated(const zaf::DeactivatedInfo& event_info) override;
    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;

private:
    void ResizeToSuitableSize();

private:
    ZAF_BIND_CONTROL(zaf::ScrollableControl, scrollableControl);
    ZAF_BIND_CONTROL(zaf::RichEdit, textEdit);

    zaf::Subject<std::shared_ptr<TextBlockWindow>> text_changed_event_;
};

}

