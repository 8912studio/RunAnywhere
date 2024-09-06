#include "main/main_window.h"
#include <cassert>
#include <zaf/base/log.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>
#include <zaf/creation.h>
#include <zaf/graphic/dpi.h>
#include <zaf/window/message/hit_test_message.h>
#include <zaf/window/message/hit_test_result.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/message/message.h>
#include "context/desktop_context_discovering.h"
#include "help/help_content_building.h"
#include "utility/markdown/element/factory.h"
#include "main/preserve/preserved_command_view.h"
#include "main/preview_control_creating.h"
#include "module/active_path/active_path_module.h"
#include "module/calculator/calculator_module.h"
#include "module/common/copy_executor.h"
#include "module/meta/meta_module.h"
#include "module/user_defined/user_defined_module.h"
#include "option/option_storage.h"

namespace ra {

ZAF_OBJECT_IMPL(MainWindow);

MainWindow::MainWindow(const std::shared_ptr<ModuleManager>& module_manager) : 
    module_manager_(module_manager) {

}


void MainWindow::AfterParse() {

    __super::AfterParse();

    initial_height_ = this->Height();

    InitializeTextBox();
    InitializeHelpButton();
    InitializeToolbar();

    Subscriptions() += preservedCommandListView->RectChangedEvent().Subscribe(
        [this](const zaf::RectChangedInfo& event_info) {
    
        if (preservedCommandListView->IsVisibleInContext() && 
            (preservedCommandListView->Height() != event_info.PreviousRect().size.height)) {

            UpdateWindowRect();
        }
    });
}


void MainWindow::InitializeTextBox() {

    inputEdit->SetStyle(CommandDisplayStyle::Normal);

    Subscriptions() += inputEdit->CommandChangedEvent().Subscribe(
        std::bind(&MainWindow::OnCommandChanged, this));
}


void MainWindow::InitializeHelpButton() {

    Subscriptions() += helpButton->MouseUpEvent().Subscribe(
        std::bind(&MainWindow::OnHelpButtonClick, this));
}


void MainWindow::InitializeToolbar() {
    
    Subscriptions() += toolbar->ExecuteEvent().Subscribe(std::bind([this]() {
        ExecuteCommand();
    }));

    Subscriptions() += toolbar->PreserveEvent().Subscribe(std::bind([this]() {
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

    AdjustPositionBeforeShow();
    this->Show();
    SetForegroundWindow(this->Handle());
}


void MainWindow::AdjustPositionBeforeShow() {

    if (!this->Handle()) {
        AdjustPositionOnFirstShow();
    }
    else {
        EnsureInVisibleArea();
    }
}


void MainWindow::AdjustPositionOnFirstShow(){

    //First show, center the window in screen of primary monitor.
    RECT work_area_rect{};
    BOOL is_succeeded = SystemParametersInfo(SPI_GETWORKAREA, 0, &work_area_rect, 0);
    if (!is_succeeded) {
        return;
    }

    auto window_size = this->Size();
    auto dpi = this->GetDPI();

    auto screen_width = zaf::ToDIPs(
        static_cast<float>(work_area_rect.right - work_area_rect.left),
        dpi);

    auto screen_height = zaf::ToDIPs(
        static_cast<float>(work_area_rect.bottom - work_area_rect.top),
        dpi);

    zaf::Point position{
        (screen_width - window_size.width) / 2,
        //Make the window a bit higher
        (screen_height - window_size.height) / 2 - window_size.height * 2
    };

    this->SetPosition(position);
}


void MainWindow::EnsureInVisibleArea() {

    //Ensure the window is in visible area of the nearest monitor.
    auto adjusted_rect = EnsureRectInVisibleArea(this->Rect());
    this->SetPosition(adjusted_rect.position);
}


zaf::Rect MainWindow::EnsureRectInVisibleArea(const zaf::Rect& rect) const {

    HMONITOR monitor = MonitorFromWindow(this->Handle(), MONITOR_DEFAULTTONEAREST);

    MONITORINFO monitor_info{};
    monitor_info.cbSize = sizeof(monitor_info);
    BOOL is_succeeded = GetMonitorInfo(monitor, &monitor_info);
    if (!is_succeeded) {
        return rect;
    }

    auto screen_rect = zaf::ToDIPs(zaf::Rect::FromRECT(monitor_info.rcWork), this->GetDPI());
    auto result = rect;

    //Adjust horizontal coordinations.
    if (result.Left() < screen_rect.Left()) {
        result.position.x = screen_rect.Left();
    }
    else if (result.Right() >= screen_rect.Right()) {
        result.position.x = screen_rect.Right() - result.size.width;
    }

    //Adjust vertical coordinations.
    if (result.Top() < screen_rect.Top()) {
        result.position.y = screen_rect.Top();
    }
    else if (result.Bottom() >= screen_rect.Bottom()) {
        result.position.y = screen_rect.Bottom() - result.size.height;
    }

    return result;
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
        if (current_command_->GetKeyword() == command_line.Command()) {

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

    toolbar->UpdateStyle(CommandDisplayStyle::Normal, current_command_->GetExecutor());
    toolbar->SetIsVisible(true);
}


void MainWindow::ShowEmptyPreview() {

    previewView->ClearPreviewControl();
    previewView->SetIsVisible(false);
    toolbar->SetIsVisible(false);
}


void MainWindow::UpdateWindowRect() {

    float main_height = initial_height_;
    if (previewView->IsVisibleInContext()) {
        main_height += previewView->GetExpectedHeight();
    }

    if (toolbar->IsVisibleInContext()) {
        main_height += toolbar->Height();
    }

    float preserved_commands_view_height{};
    if (preservedCommandListView->IsVisibleInContext()) {
        preserved_commands_view_height = preservedCommandListView->Height();
    }

    auto window_rect = this->Rect();
    float main_y = window_rect.position.y + last_preserved_commands_view_height_;
    window_rect.position.y = main_y - preserved_commands_view_height;
    window_rect.size.height = main_height + preserved_commands_view_height;

    window_rect = EnsureRectInVisibleArea(window_rect);
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


help::HelpContent MainWindow::GetHelpContent() {

    if (current_command_) {
        return current_command_->GetHelpContent();
    }

    auto suggested_commands = module_manager_->QuerySuggestedCommands(inputEdit->Text());
    auto element = help::BuildHelpContentFromSuggestedCommands(std::move(suggested_commands));
    return { L"suggestions", std::move(element) };
}


void MainWindow::UpdateHelpWindowPosition() {

    if (!help_window_) {
        return;
    }

    auto main_window_rect = this->Rect();
    zaf::Point help_window_position;
    constexpr float window_gap = 4;
    help_window_position.x = main_window_rect.Right() + window_gap;
    help_window_position.y = main_window_rect.position.y;
    
    if (preservedCommandListView->IsVisibleInContext()) {
        help_window_position.y += preservedCommandListView->Height();
    }

    help_window_->SetPosition(help_window_position);
}


void MainWindow::ExecuteCommand() {

    mod::PostExecuteAction post_execute_action{ mod::PostExecuteAction::Dispose };

    if (current_command_) {
        auto executor = current_command_->GetExecutor();
        if (executor) {
            post_execute_action = executor->Execute().PostExecuteAction();
        }
    }

    if (post_execute_action == mod::PostExecuteAction::Preserve) {
        PreserveCurrentCommand();
        return;
    }

    if (!option::OptionStorage::Instance().RememberLastCommand()) {
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
    
        preservedCommandListView->RemoveView(view);
    });

    preservedCommandListView->AddView(preserved_command_view);
    preservedCommandListView->SetIsVisible(true);

    inputEdit->SetText({});
}


void MainWindow::RemoveTheFirstPreservedCommand() {
    preservedCommandListView->RemoveFirstView();
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

    if (message.Key() == zaf::Key::Escape) {

        if (inputEdit->Text().empty()) {
            this->Hide();
        }
        else {
            inputEdit->SetText(std::wstring{});
        }
        return true;
    }

    if (message.Key() == zaf::Key::Enter) {
        if (GetKeyState(VK_SHIFT) >> 15) {
            PreserveCurrentCommand();
        }
        else {
            ExecuteCommand();
        }
        return true;
    }

    if (message.Key() == zaf::Key::Backspace) {
        if (GetKeyState(VK_SHIFT) >> 15) {
            RemoveTheFirstPreservedCommand();
            return true;
        }
    }

    if (message.Key() == static_cast<zaf::Key>(VK_OEM_2)) {
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

    switch (message.Key()) {
    case zaf::Key::J:
        help_window_->ScrollLine(false);
        return true;
    case zaf::Key::K:
        help_window_->ScrollLine(true);
        return true;
    case zaf::Key::N:
        help_window_->ScrollPage(false);
        return true;
    case zaf::Key::U:
        help_window_->ScrollPage(true);
        return true;
    default:
        return false;
    }
}


std::optional<zaf::HitTestResult> MainWindow::HitTest(const zaf::HitTestMessage& message) {

    auto mouse_position = message.MousePosition();

    if (inputEdit->RectInWindow()->Contain(mouse_position)) {
        return std::nullopt;
    }

    if (previewView->RectInWindow()->Contain(mouse_position)) {
        return std::nullopt;
    }

    if (helpButton->RectInWindow()->Contain(mouse_position)) {
        return std::nullopt;
    }

    if (preservedCommandListView->IsVisibleInContext() &&
        preservedCommandListView->RectInWindow()->Contain(mouse_position)) {
        return std::nullopt;
    }

    if (toolbar->RectInWindow()->Contain(mouse_position)) {
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

    if (!option::OptionStorage::Instance().AutoHideOnLostFocus()) {
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