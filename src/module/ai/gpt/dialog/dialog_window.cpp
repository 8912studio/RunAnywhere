#include "module/ai/gpt/dialog/dialog_window.h"
#include <zaf/creation.h>
#include <zaf/rx/thread/main_thread.h>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogWindow);

DialogWindow::DialogWindow(std::shared_ptr<Dialog> dialog) {

}


void DialogWindow::AfterParse() {

    __super::AfterParse();

    this->SetContentSize(zaf::Size{ 600, 600 });
    this->SetMinSize(zaf::Size{ 300, 300 });
    this->SetRootControl(view_);

    this->SetTitle(view_->Subject());
    Disposables() += view_->SubjectUpdatedEvent().Subscribe(std::bind([this]() {
        this->SetTitle(view_->Subject());
    }));
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
    Disposables() += zaf::rx::MainThread::Instance()->PostWork([this]() {
        view_->SetFocusToInputEdit();
    });
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