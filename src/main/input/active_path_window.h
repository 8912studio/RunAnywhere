#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include "main/input/argument_object_window.h"

namespace ra::main::input {

class ActivePathWindow : public ArgumentObjectWindow {
public:
    ZAF_DECLARE_TYPE;

public:
    void SetIsReadOnly(bool is_read_only) override;
    std::wstring GetText() override;
    void SetText(const std::wstring& text) override;

protected:
    void AfterParse() override;
    void OnShow(const zaf::ShowInfo& event_info) override;
    zaf::Size CalculateWindowContentSize() override;

private:
    ZAF_BIND_CONTROL(zaf::Control, container);
    ZAF_BIND_CONTROL(zaf::Control, label);
    ZAF_BIND_CONTROL(zaf::Control, scrollContainer);
    ZAF_BIND_CONTROL(zaf::RichEdit, edit);
};

}