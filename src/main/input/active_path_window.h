#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include "main/input/argument_object_window.h"

namespace ra::main::input {

class ActivePathWindow : public ArgumentObjectWindow {
public:
    ZAF_OBJECT;

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

ZAF_OBJECT_BEGIN(ActivePathWindow)
ZAF_OBJECT_RESOURCE_URI(L"res:///main/input/active_path_window.xaml")
ZAF_OBJECT_END;

}