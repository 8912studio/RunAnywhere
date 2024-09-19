#include "module/ai/gpt/dialog/dialog_window.h"
#include <zaf/creation.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogWindow);

DialogWindow::DialogWindow(std::shared_ptr<Dialog> dialog) : 
    view_(zaf::Create<DialogView>(std::move(dialog))) {

}


void DialogWindow::AfterParse() {

    __super::AfterParse();

    this->SetTitle(L"Chat with GPT");
    this->SetContentSize(zaf::Size{ 600, 600 });
    this->SetMinSize(zaf::Size{ 300, 300 });
    this->SetRootControl(view_);
}


void DialogWindow::OnMessageReceived(const zaf::MessageReceivedInfo& event_info) {

    if (event_info.Message().ID() == WM_KEYDOWN) {
        if (HandleKeyDownMessage(zaf::KeyMessage{ event_info.Message() })) {
            event_info.MarkAsHandled(0);
        }
    }

    __super::OnMessageReceived(event_info);
}


bool DialogWindow::HandleKeyDownMessage(const zaf::KeyMessage& message) {

    if (message.Key() == zaf::Key::Escape) {
        this->Close();
        return true;
    }

    return false;
}


void DialogWindow::Open() {

    OpenWindow();

    //When the window is shown for the first time, there is no window focus by the time this method
    //is called. Therefore, we set focus to the inputEdit in the next message loop, after the 
    //window gets focus.
    Subscriptions() += zaf::rx::Empty().ObserveOn(zaf::Scheduler::Main()).DoOnTerminated([this]() {
        view_->SetFocusToInputEdit();
    })
    .Subscribe();
}


void DialogWindow::OpenWindow() {

    if (!Handle()) {
        Show();
        return;
    }

    if (IsIconic(Handle())) {
        Restore();
        return;
    }

    SetForegroundWindow(Handle());
    BringWindowToTop(Handle());
}

}