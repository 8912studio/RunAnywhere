#pragma once

#include <zaf/control/control_binder.h>
#include <zaf/control/text_box.h>
#include <zaf/window/message/activate_message.h>
#include <zaf/window/window.h>
#include "context/desktop_context.h"
#include "help/help_window.h"
#include "module/command_preview_control.h"
#include "module_manager.h"
#include "preview_view.h"

namespace ra {

class MainWindow : public zaf::Window {
public:
    ZAF_DECLARE_TYPE;

    static MainWindow& Instance();

    void ShowOnTop();
    void ReloadUserDefinedCommands();

protected:
    void AfterParse() override;

    bool HandleMessage(const zaf::Message& message, LRESULT& result) override;
    std::optional<zaf::HitTestResult> HitTest(const zaf::HitTestMessage& message) override;
    void OnWindowShown() override;

private:
    void InitializeTextBox();
    void InitializeHelpButton();
    void InitializeModules();

    void OnTextChanged(const zaf::TextualControlTextChangeInfo&);
    void UpdateCommandState();
    void InterpretCommand(const std::wstring& input);
    void ShowPreview();
    float ShowCommandPreview();
    std::shared_ptr<module::CommandPreviewControl> CreateDefaultPreviewControl(
        const std::wstring& preview_text);
    float ShowEmptyPreview();
    void OnHelpButtonClick();
    void UpdateHelpWindowState();
    void ShowHelpWindow();
    help::content::Content GetHelpContent();
    void UpdateHelpWindowPosition();
    void ExecuteCommand();

    bool ReceiveKeyDownMessage(const zaf::KeyMessage& message);
    bool HandleHelpWindowScrollMessage(const zaf::KeyMessage& message);
    void HandleActivateMessage(const zaf::ActivateMessage& message);

private:
    ZAF_BIND_CONTROL(zaf::TextBox, inputTextBox);
    ZAF_BIND_CONTROL(PreviewView, previewView);
    ZAF_BIND_CONTROL(zaf::Control, helpButton);

    float initial_height_{};

    ModuleManager module_manager_;

    context::DesktopContext desktop_context_;
    std::shared_ptr<module::Command> current_command_;

    bool show_help_window_{};
    std::shared_ptr<help::HelpWindow> help_window_;
};

}