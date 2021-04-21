#include "main_window.h"
#include <zaf/base/log.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/base/string/trim.h>
#include <zaf/control/label.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include <zaf/reflection/reflection_type_definition.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/message/message.h>
#include "context/desktop_context_discovering.h"
#include "module/calculator/calculator_module.h"
#include "module/meta/meta_module.h"
#include "module/user_defined/user_defined_module.h"

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

    user_defined_module_ = std::make_shared<UserDefinedModule>();
    user_defined_module_->Reload();

    modules_.push_back(std::make_shared<MetaModule>());
    modules_.push_back(user_defined_module_);
    modules_.push_back(std::make_shared<CalculatorModule>());
}


void MainWindow::ShowOnTop() {

    desktop_context_ = DiscoverDesktopContext();

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


std::shared_ptr<CommandPreviewControl> MainWindow::CreateDefaultPreviewControl(
    const std::wstring& preview_text) {

    auto label = zaf::Create<zaf::Label>();
    label->SetFixedHeight(50);
    label->SetTextAlignment(zaf::TextAlignment::Leading);
    label->SetParagraphAlignment(zaf::ParagraphAlignment::Center);
    label->SetFontSize(20);
    label->SetText(preview_text);

    auto result = zaf::Create<CommandPreviewControl>();
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


void MainWindow::OnWindowShown() {

    __super::OnWindowShown();

    inputTextBox->SetIsFocused(true);

    InterpretCommand(inputTextBox->GetText());
}


