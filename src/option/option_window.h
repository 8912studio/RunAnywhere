#pragma once

#include <zaf/control/button.h>
#include <zaf/control/check_box.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/label.h>
#include <zaf/control/rich_edit.h>
#include <zaf/control/spin_box.h>
#include <zaf/window/dialog.h>
#include "hot_key_box.h"
#include "option/option_text_box.h"

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

    void InitializeMaxPreservedCount();
    void InitializeOpenAIAPIKey();
    void InitializeProxy();

    void UpdateAutoRunCheckBoxState();
    bool CheckIfSetAutoRun();
    void OnAutoRunCheckBoxStateChanged();
    void SetAutoRunToRegistry(bool set);

    void UpdateAutoHideCheckBoxState();
    void OnAutoHideCheckBoxStateChanged();

    void UpdateRememberLastCommandCheckBoxState();
    void OnRememberLastCommandCheckBoxStateChanged();

    void OnRegisterFileAssociationButtonClick();

private:
    ZAF_BIND_CONTROL(HotKeyBox, hotKeyBox);
    ZAF_BIND_CONTROL(zaf::Label, hotKeyInvalidLabel);
    ZAF_BIND_CONTROL(zaf::SpinBox, maxPreservedCount);
    ZAF_BIND_CONTROL(zaf::CheckBox, autoRunCheckBox);
    ZAF_BIND_CONTROL(zaf::CheckBox, autoHideCheckBox);
    ZAF_BIND_CONTROL(zaf::CheckBox, rememberLastCommandCheckBox);
    ZAF_BIND_CONTROL(option::OptionTextBox, apiKeyEdit);
    ZAF_BIND_CONTROL(option::OptionTextBox, proxyEdit);
    ZAF_BIND_CONTROL(zaf::Button, registerFileAssociationButton);
};

}