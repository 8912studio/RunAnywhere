#pragma once

#include <zaf/control/check_box.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/window/dialog.h>
#include "hot_key_box.h"

namespace ra {

class OptionWindow : public zaf::Dialog {
public:
    ZAF_DECLARE_TYPE;

    static void ShowInstance();

protected:
    void AfterParse() override;

private:
    void OnHotKeyChanged(const HotKey& hot_key);
    void CheckIfHotKeyValid();

    void CheckAutoRunCheckBoxState();
    bool CheckIfSetAutoRun();
    void OnAutoRunCheckBoxStateChanged();
    void SetAutoRunToRegistry(bool set);

private:
    ZAF_BIND_CONTROL(HotKeyBox, hotKeyBox);
    ZAF_BIND_CONTROL(zaf::Label, hotKeyInvalidLabel);
    ZAF_BIND_CONTROL(zaf::CheckBox, autoRunCheckBox);
};

}