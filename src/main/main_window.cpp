#include "main/main_window.h"
#include <cassert>
#include <zaf/base/log.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>
#include <zaf/creation.h>
#include <zaf/graphic/dpi.h>
#include <zaf/object/type_definition.h>
#include <zaf/window/message/hit_test_message.h>
#include <zaf/window/message/hit_test_result.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/message/message.h>
#include "context/desktop_context_discovering.h"
#include "help/help_content_building.h"
#include "main/preserve/preserved_command_view.h"
#include "main/preview_control_creating.h"
#include "module/active_path/active_path_module.h"
#include "module/calculator/calculator_module.h"
#include "module/common/copy_executor.h"
#include "module/meta/meta_module.h"
#include "module/user_defined/user_defined_module.h"
#include "option_storage.h"

namespace ra {

ZAF_DEFINE_TYPE(MainWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/main_window.xaml")
ZAF_DEFINE_TYPE_END


MainWindow::MainWindow(const std::shared_ptr<ModuleManager>& module_manager) : 
    module_manager_(module_manager) {

}


void MainWindow::AfterParse() {

    __super::AfterParse();

    initial_height_ = this->Height();

    InitializeTextBox();
    InitializeHelpButton();
    InitializeToolbar();
}


void MainWindow::InitializeTextBox() {

    inputEdit->SetAllowBeep(false);
    inputEdit->SetStyle(CommandDisplayStyle::Normal);

    Subscriptions() += inputEdit->CommandChangedEvent().Subscribe(
        std::bind(&MainWindow::OnCommandChanged, this));
}


void MainWindow::InitializeHelpButton() {

    Subscriptions() += helpButton->MouseUpEvent().Subscribe(
        std::bind(&MainWindow::OnHelpButtonClick, this));
}


void MainWindow::InitializeToolbar() {
    
    Subscriptions() += executeButton->ClickEvent().Subscribe(std::bind([this]() {
        ExecuteCommand();
    }));

    Subscriptions() += preserveButton->ClickEvent().Subscribe(std::bind([this]() {
        PreserveCurrentCommand();
    }));
}


void MainWindow::ShowOnTop() {

    desktop_context_ = context::DesktopContext{}; 

    Subscriptions() += context::DiscoverDesktopContext().Subscribe(
        [this](const context::DesktopContext& context) {
    
        desktop_context_ = context;
        UpdateCommandState();
    });

    //First show, center the window in screen
    if (!this->Handle()) {

        auto size = this->Size();
        auto dpi = GetDPI();

        auto screen_width = zaf::ToDIPs(static_cast<float>(GetSystemMetrics(SM_CXSCREEN)), dpi);
        auto screen_height = zaf::ToDIPs(static_cast<float>(GetSystemMetrics(SM_CYSCREEN)), dpi);
        zaf::Point position(
            (screen_width - size.width) / 2,
            (screen_height - size.height) / 2 - size.height * 2); //Make the window a bit higher

        this->SetPosition(position);
    }

    this->Show();
    SetForegroundWindow(this->Handle());
}


void MainWindow::OnCommandChanged() {

    UpdateCommandState();
}


void MainWindow::UpdateCommandState() {

    InterpretCommand();
    ShowPreview();
    UpdateHelpWindowState();
}


void MainWindow::InterpretCommand() {

    auto command_line = inputEdit->GetInputCommandLine();

    //Try to reuse current command.
    if (current_command_) {
        if (current_command_->GetBrief().Command() == command_line.Command()) {

            if (current_command_->Interpret(command_line, desktop_context_, true)) {
                return;
            }
        }
    }

    current_command_.reset();

    //Cannot reuse, create a new command.
    auto new_command = module_manager_->CreateCommand(command_line);
    if (new_command) {

        if (new_command->Interpret(command_line, desktop_context_, false)) {
            current_command_ = std::move(new_command);
        }
    }
}


void MainWindow::ShowPreview() {

    if (current_command_) {
        ShowCommandPreview();
    }
    else {
        ShowEmptyPreview();
    }

    UpdateWindowRect();
}


void MainWindow::ShowCommandPreview() {

    auto preview_control = CreateCommandPreviewControl(*current_command_);
    preview_control->SetStyle(CommandDisplayStyle::Normal);
    
    previewView->SetPreviewControl(preview_control);
    previewView->SetIsVisible(true);

    toolbar->SetIsVisible(true);

    UpdateExecuteButton();
}


void MainWindow::UpdateExecuteButton() {

    auto executor = current_command_->GetExecutor();
    if (!executor) {
        executeButton->SetIsVisible(false);
        return;
    }

    if (zaf::As<mod::CopyExecutor>(executor)) {
        executeButton->SetImageName(L"copy");
        executeButton->SetTooltip(L"Copy result to clipboard (ENTER)");
    }
    else {
        executeButton->SetImageName(L"execute");
        executeButton->SetTooltip(L"Execute current command (ENTER)");
    }

    executeButton->SetIsVisible(true);
}


void MainWindow::ShowEmptyPreview() {

    previewView->ClearPreviewControl();
    previewView->SetIsVisible(false);
    toolbar->SetIsVisible(false);
}


void MainWindow::UpdateWindowRect() {

    float main_height = initial_height_;
    if (previewView->IsVisible()) {
        main_height += previewView->GetExpectedHeight();
    }

    if (toolbar->IsVisible()) {
        main_height += toolbar->Height();
    }

    float preserved_commands_view_height{};
    if (preservedCommandsView->IsVisible()) {
        preserved_commands_view_height = preservedCommandsView->Height();
    }

    auto window_rect = this->Rect();
    float main_y = window_rect.position.y + last_preserved_commands_view_height_;
    window_rect.position.y = main_y - preserved_commands_view_height;
    window_rect.size.height = main_height + preserved_commands_view_height;
    this->SetRect(window_rect);

    last_preserved_commands_view_height_ = preserved_commands_view_height;
}


void MainWindow::OnHelpButtonClick() {

    show_help_window_ = !show_help_window_;

    std::wstring button_image_uri = 
        show_help_window_ ? 
        L"res:///resource/help_active.png" : 
        L"res:///resource/help_inactive.png";

    helpButton->SetBackgroundImage(zaf::Image::FromURI(button_image_uri));

    UpdateHelpWindowState();
}


void MainWindow::UpdateHelpWindowState() {

    if (show_help_window_ && IsWindowVisible(this->Handle())) {
        ShowHelpWindow();
    }
    else {
        if (help_window_) {
            help_window_->Hide();
        }
    }
}


void MainWindow::ShowHelpWindow() {

    if (!help_window_) {
        help_window_ = zaf::Create<help::HelpWindow>();
        help_window_->SetOwner(this->shared_from_this());
    }

    help_window_->SetContent(GetHelpContent());

    UpdateHelpWindowPosition();
    help_window_->Show();
}


help::content::Content MainWindow::GetHelpContent() {

    if (current_command_) {

        auto result = current_command_->GetHelpContent();
        if (result.Lines().empty()) {
            result.AddBodyLine(L"No help information for this command");
        }
        return result;
    }
    else {

        auto suggested_commands = module_manager_->QuerySuggestedCommands(inputEdit->Text());
        return help::BuildHelpContentFromSuggestedCommands(std::move(suggested_commands));
    }
}


void MainWindow::UpdateHelpWindowPosition() {

    if (!help_window_) {
        return;
    }

    auto main_window_rect = this->Rect();
    zaf::Point help_window_position;
    constexpr float window_gap = 4;
    help_window_position.x = main_window_rect.position.x + main_window_rect.size.width + window_gap;
    help_window_position.y = main_window_rect.position.y;

    help_window_->SetPosition(help_window_position);
}


void MainWindow::ExecuteCommand() {

    if (current_command_) {
        auto executor = current_command_->GetExecutor();
        if (executor) {
            executor->Execute();
        }
    }

    if (!OptionStorage::Instance().RememberLastCommand()) {
        inputEdit->SetText({});
    }
    this->Hide();
}


void MainWindow::PreserveCurrentCommand() {

    if (!current_command_) {
        return;
    }

    auto preserved_command_view = zaf::Create<PreservedCommandView>(
        inputEdit->GetInputContent(),
        std::move(current_command_));

    Subscriptions() += preserved_command_view->CloseEvent().Subscribe(
        [this](const std::shared_ptr<PreservedCommandView>& view) {
    
        preservedCommandsView->RemoveChild(view);
        UpdateWindowRect();
    });

    {
        auto update_guard = preservedCommandsView->BeginUpdate();

        if (preservedCommandsView->ChildCount() >= 3) {
            preservedCommandsView->RemoveChildAtIndex(0);
        }

        preservedCommandsView->AddChild(preserved_command_view);
        preservedCommandsView->SetIsVisible(true);
    }

    inputEdit->SetText({});
}


void MainWindow::RemoveTheFirstPreservedCommand() {

    if (preservedCommandsView->ChildCount() == 0) {
        return;
    }

    preservedCommandsView->RemoveChildAtIndex(0);
    UpdateWindowRect();
}


void MainWindow::OnMessageReceived(const zaf::MessageReceivedInfo& event_info) {

    auto handle_result = HandleMessage(event_info.Message());
    if (handle_result) {
        event_info.MarkAsHandled(*handle_result);
    }

    __super::OnMessageReceived(event_info);
}


std::optional<LRESULT> MainWindow::HandleMessage(const zaf::Message& message) {

    if (message.ID() == WM_KEYDOWN) {
        if (HandleKeyDownMessage(zaf::KeyMessage{ message })) {
            return 0;
        }
    }
    else if (message.ID() == WM_SHOWWINDOW) {

        if (!message.WParam()) {
            if (help_window_) {
                help_window_->Hide();
            }
        }
    }
    else if (message.ID() == WM_MOVE) {

        UpdateHelpWindowPosition();
    }

    return std::nullopt;
}


bool MainWindow::HandleKeyDownMessage(const zaf::KeyMessage& message) {

    if (message.VirtualKey() == VK_ESCAPE) {

        if (inputEdit->Text().empty()) {
            this->Hide();
        }
        else {
            inputEdit->SetText(std::wstring{});
        }
        return true;
    }

    if (message.VirtualKey() == VK_RETURN) {
        if (GetKeyState(VK_SHIFT) >> 15) {
            PreserveCurrentCommand();
        }
        else {
            ExecuteCommand();
        }
        return true;
    }

    if (message.VirtualKey() == VK_BACK) {
        if (GetKeyState(VK_SHIFT) >> 15) {
            RemoveTheFirstPreservedCommand();
            return true;
        }
    }

    if (message.VirtualKey() == VK_OEM_2) {
        if ((GetKeyState(VK_CONTROL) >> 15) != 0) {
            OnHelpButtonClick();
            return true;
        }
    }

    return HandleHelpWindowScrollMessage(message);
}


bool MainWindow::HandleHelpWindowScrollMessage(const zaf::KeyMessage& message) {

    if (!show_help_window_ || !help_window_) {
        return false;
    }

    if ((GetKeyState(VK_CONTROL) >> 15) == 0) {
        return false;
    }

    switch (message.VirtualKey()) {
    case L'J':
        help_window_->ScrollLine(false);
        return true;
    case L'K':
        help_window_->ScrollLine(true);
        return true;
    case L'N':
        help_window_->ScrollPage(false);
        return true;
    case L'U':
        help_window_->ScrollPage(true);
        return true;
    default:
        return false;
    }
}


std::optional<zaf::HitTestResult> MainWindow::HitTest(const zaf::HitTestMessage& message) {

    auto mouse_position = message.MousePosition();

    if (inputEdit->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    if (previewView->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    if (helpButton->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    if (preservedCommandsView->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    if (toolbar->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    return zaf::HitTestResult::TitleBar;
}


void MainWindow::OnShow(const zaf::ShowInfo& event_info) {

    __super::OnShow(event_info);

    inputEdit->SetSelectionRange(zaf::Range{ 0, inputEdit->TextLength() });

    UpdateCommandState();
}


void MainWindow::OnFocusGained(const zaf::WindowFocusGainedInfo& event_info) {

    __super::OnFocusGained(event_info);

    inputEdit->SetIsFocused(true);
}


void MainWindow::OnDeactivated(const zaf::DeactivatedInfo& event_info) {

    __super::OnDeactivated(event_info);

    if (!OptionStorage::Instance().AutoHideOnLostFocus()) {
        return;
    }

    bool should_hide{ true };

    HWND current_hwnd = event_info.Message().EffectingWindowHandle();
    while (current_hwnd) {

        current_hwnd = GetWindow(current_hwnd, GW_OWNER);
        if (current_hwnd == this->Handle()) {
            should_hide = false;
            break;
        }
    }

    if (should_hide) {
        this->Hide();
    }
}

}