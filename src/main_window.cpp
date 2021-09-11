#include "main_window.h"
#include <zaf/base/log.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>
#include <zaf/control/label.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>
#include <zaf/window/message/activate_message.h>
#include <zaf/window/message/hit_test_message.h>
#include <zaf/window/message/hit_test_result.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/message/message.h>
#include "context/desktop_context_discovering.h"
#include "help/help_content_building.h"
#include "module/active_path/active_path_module.h"
#include "module/calculator/calculator_module.h"
#include "module/crypto/crypto_module.h"
#include "module/date/date_module.h"
#include "module/meta/meta_module.h"
#include "module/rgb/rgb_module.h"
#include "module/user_defined/user_defined_module.h"
#include "utility/path_trimming.h"

namespace ra {

ZAF_DEFINE_TYPE(MainWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main_window.xaml")
ZAF_DEFINE_TYPE_END

MainWindow& MainWindow::Instance() {

    static auto instance = zaf::Create<MainWindow>();
    return *instance;
}


void MainWindow::AfterParse() {

    __super::AfterParse();

    initial_height_ = this->Height();

    InitializeTextBox();
    InitializeHelpButton();
    InitializeModules();
}


void MainWindow::InitializeTextBox() {

    inputTextBox->SetAllowBeep(false);

    Subscriptions() += inputTextBox->TextChangeEvent().Subscribe(std::bind(
        &MainWindow::OnTextChanged,
        this,
        std::placeholders::_1));
}


void MainWindow::InitializeHelpButton() {

    Subscriptions() += helpButton->ClickEvent().Subscribe(
        std::bind(&MainWindow::OnHelpButtonClick, this));
}


void MainWindow::InitializeModules() {

    module_manager_.Initialize();
}


void MainWindow::ShowOnTop() {

    desktop_context_ = context::DiscoverDesktopContext();

    this->Show();
    SetForegroundWindow(this->Handle());
}


void MainWindow::ReloadUserDefinedCommands() {

    module_manager_.ReloadUserDefinedCommands();
}


void MainWindow::OnTextChanged(const zaf::TextualControlTextChangeInfo& event_info) {

    UpdateCommandState();
}


void MainWindow::UpdateCommandState() {

    InterpretCommand(inputTextBox->Text());
    UpdateHelpWindowState();
}


void MainWindow::InterpretCommand(const std::wstring& input) {

    current_command_ = module_manager_.InterpretCommand(input);
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

    if (show_help_window_) {
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

    auto suggested_commands = module_manager_.QuerySuggestedCommands(inputTextBox->Text());
    auto help_content = help::BuildHelpContentFromSuggestedCommands(std::move(suggested_commands));
    help_window_->SetContent(help_content);

    UpdateHelpWindowPosition();
    help_window_->Show();
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


bool MainWindow::ReceiveMessage(const zaf::Message& message, LRESULT& result) {

    if (message.id == WM_KEYDOWN) {
        if (ReceiveKeyDownMessage(zaf::As<zaf::KeyMessage>(message))) {
            return true;
        }
    }
    else if (message.id == WM_ACTIVATE) {

        const auto& activate_message = zaf::As<zaf::ActivateMessage>(message);
        if (activate_message.State() == zaf::ActivateState::Inactive) {

            bool should_hide{ true };

            HWND current_hwnd = activate_message.EffectingWindowHandle();
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
    else if (message.id == WM_SHOWWINDOW) {

        if (!message.wparam) {
            if (help_window_) {
                help_window_->Hide();
            }
        }
    }
    else if (message.id == WM_MOVE) {

        UpdateHelpWindowPosition();
    }

    return __super::ReceiveMessage(message, result);
}


bool MainWindow::ReceiveKeyDownMessage(const zaf::KeyMessage& message) {

    if (message.GetVirtualKey() == VK_ESCAPE) {

        if (inputTextBox->Text().empty()) {
            this->Hide();
        }
        else {
            inputTextBox->SetText(std::wstring{});
        }
        return true;
    }

    if (message.GetVirtualKey() == VK_RETURN) {
        this->ExecuteCommand();
        return true;
    }

    if (message.GetVirtualKey() == VK_OEM_2) {
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

    switch (message.GetVirtualKey()) {
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

    auto mouse_position = message.GetMousePosition();

    if (inputTextBox->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    if (previewView->AbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    return zaf::HitTestResult::TitleBar;
}


void MainWindow::OnWindowShown() {

    __super::OnWindowShown();

    inputTextBox->SetIsFocused(true);

    UpdateCommandState();
}

}