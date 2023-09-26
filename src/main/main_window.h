#pragma once

#include <zaf/control/button.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/rich_edit.h>
#include <zaf/window/message/activate_message.h>
#include <zaf/window/window.h>
#include "context/desktop_context.h"
#include "help/help_window.h"
#include "help/markdown/element/element.h"
#include "main/input/command_input_edit.h"
#include "main/toolbar/command_toolbar.h"
#include "main/toolbar/toolbar_button.h"
#include "module/command_preview_control.h"
#include "module_manager.h"
#include "preview_view.h"
#include "utility/thin_border_window.h"

namespace ra {

class MainWindow : public utility::ThinBorderWindow {
public:
    ZAF_DECLARE_TYPE;

    explicit MainWindow(const std::shared_ptr<ModuleManager>& module_manager);

    void ShowOnTop();

protected:
    void AfterParse() override;

    void OnMessageReceived(const zaf::MessageReceivedInfo& event_info) override;
    std::optional<zaf::HitTestResult> HitTest(const zaf::HitTestMessage& message) override;
    void OnShow(const zaf::ShowInfo& event_info) override;
    void OnFocusGained(const zaf::WindowFocusGainedInfo& event_info) override;
    void OnDeactivated(const zaf::DeactivatedInfo& event_info) override;

private:
    void InitializeTextBox();
    void InitializeHelpButton();
    void InitializeToolbar();

    void OnCommandChanged();
    void UpdateCommandState();
    void InterpretCommand();
    void ShowPreview();
    void ShowCommandPreview();
    void ShowEmptyPreview();
    void UpdateWindowRect();
    void OnHelpButtonClick();
    void UpdateHelpWindowState();
    void ShowHelpWindow();
    std::shared_ptr<help::markdown::element::Element> GetHelpContent();
    void UpdateHelpWindowPosition();
    void ExecuteCommand();
    void PreserveCurrentCommand();
    void RemoveTheFirstPreservedCommand();

    std::optional<LRESULT> HandleMessage(const zaf::Message& message);
    bool HandleKeyDownMessage(const zaf::KeyMessage& message);
    bool HandleHelpWindowScrollMessage(const zaf::KeyMessage& message);

private:
    ZAF_BIND_CONTROL(main::input::CommandInputEdit, inputEdit);
    ZAF_BIND_CONTROL(PreviewView, previewView);
    ZAF_BIND_CONTROL(zaf::Control, helpButton);
    ZAF_BIND_CONTROL(zaf::Control, preservedCommandsView);
    ZAF_BIND_CONTROL(CommandToolbar, toolbar);

    float initial_height_{};

    std::shared_ptr<ModuleManager> module_manager_;

    context::DesktopContext desktop_context_;
    std::unique_ptr<mod::Command> current_command_;
    float last_preserved_commands_view_height_{};

    bool show_help_window_{};
    std::shared_ptr<help::HelpWindow> help_window_;
};

}