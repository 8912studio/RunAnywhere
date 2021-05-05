#include "main_window.h"
#include <zaf/base/log.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>
#include <zaf/control/label.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include <zaf/reflection/reflection_type_definition.h>
#include <zaf/window/message/hit_test_message.h>
#include <zaf/window/message/hit_test_result.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/message/message.h>
#include "context/desktop_context_discovering.h"
#include "module/active_path/active_path_module.h"
#include "module/calculator/calculator_module.h"
#include "module/meta/meta_module.h"
#include "module/user_defined/user_defined_module.h"
#include "utility/path_trimming.h"

namespace ra {

ZAF_DEFINE_REFLECTION_TYPE(MainWindow)
ZAF_DEFINE_RESOURCE_URI(L"res:///main_window.xaml")
ZAF_DEFINE_END

MainWindow& MainWindow::Instance() {

    static auto instance = zaf::Create<MainWindow>();
    return *instance;
}


void MainWindow::AfterParsing() {

    __super::AfterParsing();

    initial_height_ = this->GetHeight();

    InitializeTextBox();
    InitializeModules();
}


void MainWindow::InitializeTextBox() {

    Subscriptions() += inputTextBox->TextChangeEvent().Subscribe(std::bind(
        &MainWindow::OnTextChanged,
        this,
        std::placeholders::_1));
}


void MainWindow::InitializeModules() {

    user_defined_module_ = std::make_shared<module::user_defined::UserDefinedModule>();
    user_defined_module_->Reload();

    modules_.push_back(std::make_shared<module::meta::MetaModule>());
    modules_.push_back(std::make_shared<module::active_path::ActivePathModule>());
    modules_.push_back(user_defined_module_);
    modules_.push_back(std::make_shared<module::calculator::CalculatorModule>());
}


void MainWindow::ShowOnTop() {

    desktop_context_ = context::DiscoverDesktopContext();

    this->Show();
    SetForegroundWindow(this->GetHandle());
}


void MainWindow::ReloadUserDefinedCommands() {

    if (user_defined_module_) {
        user_defined_module_->Reload();
    }
}


void MainWindow::OnTextChanged(const zaf::TextualControlTextChangeInfo& event_info) {
    InterpretCommand(event_info.textual_control->GetText());
}


void MainWindow::InterpretCommand(const std::wstring& input) {

    auto trimmed_input = zaf::ToTrimmed(input);
    if (!trimmed_input.empty()) {

        for (const auto& each_runner : modules_) {

            auto command = each_runner->Interpret(trimmed_input);
            if (command) {
                command->SetDesktopContext(desktop_context_);
                current_command_ = command;
                break;
            }
        }
    }
    else {

        current_command_ = nullptr;
    }

    ShowPreview();
}


void MainWindow::ShowPreview() {

    auto window_size = this->GetSize();

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
    label->SetFixedHeight(40);
    label->SetTextAlignment(zaf::TextAlignment::Leading);
    label->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    label->SetFontSize(14);
    label->SetTextTrimming(utility::CreateTextTrimmingForPath());
    label->SetText(preview_text);

    auto result = zaf::Create<module::CommandPreviewControl>();
    result->SetLayouter(zaf::Create<zaf::VerticalLayouter>());
    result->AddChild(label);
    return result;
}


float MainWindow::ShowEmptyPreview() {

    previewView->ClearPreviewControl();
    previewView->SetIsVisible(false);
    return initial_height_;
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

        const auto& key_message = dynamic_cast<const zaf::KeyMessage&>(message);
        if (key_message.GetVirtualKey() == VK_ESCAPE) {

            if (inputTextBox->GetText().empty()) {
                this->Hide();
            }
            else {
                inputTextBox->SetText(std::wstring{});
            }
            return true;
        }

        if (key_message.GetVirtualKey() == VK_RETURN) {

            this->ExecuteCommand();
            return true;
        }
    }
    else if (message.id == WM_ACTIVATE) {

        if (LOWORD(message.wparam) == WA_INACTIVE) {
            this->Hide();
        }
    }

    return __super::ReceiveMessage(message, result);
}


std::optional<zaf::HitTestResult> MainWindow::HitTest(const zaf::HitTestMessage& message) {

    auto mouse_position = message.GetMousePosition();

    if (inputTextBox->GetAbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    if (previewView->GetAbsoluteRect().Contain(mouse_position)) {
        return std::nullopt;
    }

    return zaf::HitTestResult::TitleBar;
}


void MainWindow::OnWindowShown() {

    __super::OnWindowShown();

    inputTextBox->SetIsFocused(true);

    InterpretCommand(inputTextBox->GetText());
}

}