#include "main_window.h"
#include <zaf/base/log.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/control/layout/linear_layouter.h>
#include <zaf/creation.h>
#include <zaf/reflection/reflection_type_definition.h>
#include <zaf/window/message/keyboard_message.h>
#include <zaf/window/message/message.h>
#include "module/calculator/calculator_module.h"
#include "module/meta/meta_module.h"

ZAF_DEFINE_REFLECTION_TYPE(MainWindow)
    ZAF_DEFINE_RESOURCE_URI(L"res:///main_window.xaml")
ZAF_DEFINE_END

MainWindow::~MainWindow() {

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

    runners_.push_back(std::make_shared<MetaModule>());
    runners_.push_back(std::make_shared<CalculatorModule>());
}


void MainWindow::ShowOnTop() {

    this->Show();
    SetForegroundWindow(this->GetHandle());
}


void MainWindow::OnTextChanged(const zaf::TextualControlTextChangeInfo& event_info) {

    auto text = event_info.textual_control->GetText();
    if (!text.empty()) {

        for (const auto& each_runner : runners_) {

            auto command = each_runner->Interpret(text);
            if (command) {
                current_command_ = command;
                break;
            }
        }
    }
    else {

        current_command_ = nullptr;
    }

    ShowOutput();
}


void MainWindow::ShowOutput() {

    auto window_size = this->GetSize();

    if (current_command_) {

        outputView->SetText(current_command_->GetPreviewText());
        outputView->SetIsVisible(true);

        window_size.height = 0;
        for (const auto& each_child : this->GetRootControl()->GetChildren()) {
            window_size.height += each_child->GetHeight();
        }
    }
    else {

        outputView->SetText({});
        outputView->SetIsVisible(false);

        window_size.height = initial_height_;
    }

    this->SetSize(window_size);
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

            this->Hide();
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
}


