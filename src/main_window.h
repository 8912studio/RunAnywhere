#pragma once

#include <zaf/window/window.h>
#include <zaf/control/control_binder.h>
#include <zaf/control/text_box.h>
#include "output_view.h"
#include "module/module.h"

class MainWindow : public zaf::Window {
public:
    ZAF_DECLARE_REFLECTION_TYPE;

    ~MainWindow();

    void ShowOnTop();

protected:
    void AfterParsing() override;

    bool ReceiveMessage(const zaf::Message& message, LRESULT& result) override;
    void OnWindowShown() override;

private:
    void InitializeTextBox();
    void InitializeModules();

    void OnTextChanged(const zaf::TextualControlTextChangeInfo&);
    void ShowOutput();
    void ExecuteCommand();

private:
    ZAF_BIND_CONTROL(zaf::TextBox, inputTextBox);
    ZAF_BIND_CONTROL(OutputView, outputView);

    float initial_height_{};
    std::vector<std::shared_ptr<Module>> runners_;
    std::shared_ptr<Command> current_command_;
};