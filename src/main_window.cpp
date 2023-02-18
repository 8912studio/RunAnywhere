#include "main_window.h"
#include <dwmapi.h>
#include <cassert>
#include <zaf/base/log.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>
#include <zaf/control/label.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include <zaf/graphic/dpi.h>
#include <zaf/object/type_definition.h>
#include <zaf/window/message/hit_test_message.h>
#include <zaf/window/message/hit_test_result.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/message/message.h>
#include "context/desktop_context_discovering.h"
#include "help/help_content_building.h"
#include "module/active_path/active_path_module.h"
#include "module/calculator/calculator_module.h"
#include "module/meta/meta_module.h"
#include "module/user_defined/user_defined_module.h"
#include "option_storage.h"
#include "utility/path_trimming.h"

namespace ra {

ZAF_DEFINE_TYPE(MainWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main_window.xaml")
ZAF_DEFINE_TYPE_END

MainWindow::MainWindow() {
    assert(false);
}


MainWindow::MainWindow(const std::shared_ptr<ModuleManager>& module_manager) : 
    module_manager_(module_manager) {

}


void MainWindow::AfterParse() {

    __super::AfterParse();

    initial_height_ = this->Height();

    InitializeTextBox();
    InitializeHelpButton();
}


void MainWindow::InitializeTextBox() {

    inputTextBox->SetAllowBeep(false);

    Subscriptions() += inputTextBox->TextChangedEvent().Subscribe(std::bind(
        &MainWindow::OnTextChanged,
        this,
        std::placeholders::_1));
}


void MainWindow::InitializeHelpButton() {

    Subscriptions() += helpButton->MouseUpEvent().Subscribe(
        std::bind(&MainWindow::OnHelpButtonClick, this));
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


void MainWindow::OnTextChanged(const zaf::TextChangedInfo& event_info) {

    UpdateCommandState();
}


void MainWindow::UpdateCommandState() {

    InterpretCommand(inputTextBox->Text());
    UpdateHelpWindowState();
}


void MainWindow::InterpretCommand(const std::wstring& input) {

    current_command_ = module_manager_->InterpretCommand(input);
    if (current_command_) {
        current_command_->SetDesktopContext(desktop_context_);
    }

    ShowPreview();
}


void MainWindow::ShowPreview() {

    auto window_size = this->Size();

    if (current_command_) {
        window_size.height = ShowCommandPreview();
    }
    else {
        window_size.height = ShowEmptyPreview();
    }

    this->SetSize(window_size);
}


float MainWindow::ShowCommandPreview() {

    auto preview_control = current_command_->GetPreviewControl();
    if (!preview_control) {
        preview_control = CreateDefaultPreviewControl(current_command_->GetPreviewText());
    }

    previewView->SetPreviewControl(preview_control);
    previewView->SetIsVisible(true);

    return initial_height_ + previewView->GetExpectedHeight();
}


std::shared_ptr<module::CommandPreviewControl> MainWindow::CreateDefaultPreviewControl(
    const std::wstring& preview_text) {

    auto label = zaf::Create<zaf::Label>();
    label->SetTextAlignment(zaf::TextAlignment::Leading);
    label->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    label->SetFontSize(14);
    label->SetTextTrimming(utility::CreateTextTrimmingForPath());
    label->SetText(preview_text);

    auto result = zaf::Create<module::CommandPreviewControl>();
    result->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    result->SetFixedHeight(40);
    result->AddChild(label);
    return result;
}


float MainWindow::ShowEmptyPreview() {

    previewView->ClearPreviewControl();
    previewView->SetIsVisible(false);
    return initial_height_;
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

        auto suggested_commands = module_manager_->QuerySuggestedCommands(inputTextBox->Text());
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
        current_command_->Execute();
    }

    inputTextBox->SetText({});
    this->Hide();
}


bool MainWindow::HandleMessage(const zaf::Message& message, LRESULT& result) {

    if (message.id == WM_KEYDOWN) {
        if (ReceiveKeyDownMessage(zaf::KeyMessage{ message })) {
            return true;
        }
    }
    else if (message.id == WM_ACTIVATE) {
        HandleActivateMessage(zaf::ActivateMessage{ message });
    }
    else if (message.id == WM_SHOWWINDOW) {

        if (!message.wparam) {
            if (help_window_) {
                help_window_->Hide();
            }
        }
    }
    else if (message.id == WM_NCCALCSIZE) {

        auto nc_border_in_pixels = static_cast<int>(zaf::FromDIPs(1, this->GetDPI()));

        RECT* adjusted_rect{};
        if (message.wparam == TRUE) {
            adjusted_rect = &(reinterpret_cast<NCCALCSIZE_PARAMS*>(message.lparam)->rgrc[0]);
        }
        else {
            adjusted_rect = reinterpret_cast<RECT*>(message.lparam);
        }

        adjusted_rect->left += nc_border_in_pixels;
        adjusted_rect->right -= nc_border_in_pixels;
        adjusted_rect->bottom -= nc_border_in_pixels;

        result = 0;
        return true;
    }
    else if (message.id == WM_MOVE) {

        UpdateHelpWindowPosition();
    }
    else if (message.id == WM_PAINT) {

        RECT paint_rect{};
        GetClientRect(this->Handle(), &paint_rect);
        paint_rect.bottom = static_cast<LONG>(zaf::FromDIPs(1, this->GetDPI()));

        HDC dc = GetDC(this->Handle());
        FillRect(dc, &paint_rect, reinterpret_cast<HBRUSH>(GetStockObject(BLACK_BRUSH)));
        ReleaseDC(this->Handle(), dc);
    }

    return __super::HandleMessage(message, result);
}


bool MainWindow::ReceiveKeyDownMessage(const zaf::KeyMessage& message) {

    if (message.VirtualKey() == VK_ESCAPE) {

        if (inputTextBox->Text().empty()) {
            this->Hide();
        }
        else {
            inputTextBox->SetText(std::wstring{});
        }
        return true;
    }

    if (message.VirtualKey() == VK_RETURN) {
        this->ExecuteCommand();
        return true;
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

    if (inputTextBox->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    if (previewView->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    if (helpButton->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    return zaf::HitTestResult::TitleBar;
}


void MainWindow::HandleActivateMessage(const zaf::ActivateMessage& message) {

    if (message.State() != zaf::ActivateState::Inactive) {
        return;
    }

    if (!OptionStorage::Instance().AutoHideOnLostFocus()) {
        return;
    }

    bool should_hide{ true };

    HWND current_hwnd = message.EffectingWindowHandle();
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


void MainWindow::OnWindowCreated() {

    __super::OnWindowCreated();

    MARGINS extended_margins{};
    extended_margins.cyTopHeight = static_cast<int>(zaf::FromDIPs(1, this->GetDPI()));
    DwmExtendFrameIntoClientArea(this->Handle(), &extended_margins);
}


void MainWindow::OnWindowShown() {

    __super::OnWindowShown();

    inputTextBox->SetIsFocused(true);

    UpdateCommandState();
}

}