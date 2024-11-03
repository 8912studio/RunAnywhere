#include "module/ai/gpt/dialog/unified_dialog_window.h"
#include <zaf/creation.h>

namespace ra::mod::ai::gpt {

UnifiedDialogWindow::UnifiedDialogWindow(std::shared_ptr<UnifiedDialogModel> model) {
    view_ = zaf::Create<UnifiedDialogView>(model);
}


void UnifiedDialogWindow::AfterParse() {

    __super::AfterParse();

    this->SetTitle(L"GPT");

    this->SetContentSize(zaf::Size{ 800, 600 });
    this->SetMinSize(zaf::Size{ 400, 300 });

    this->SetRootControl(view_);
}


void UnifiedDialogWindow::OnMessageReceived(const zaf::MessageReceivedInfo& event_info) {

    if (event_info.Message().ID() == WM_KEYDOWN) {
        if (HandleKeyDownMessage(zaf::KeyMessage{ event_info.Message() })) {
            event_info.MarkAsHandled(0);
        }
    }

    __super::OnMessageReceived(event_info);
}


bool UnifiedDialogWindow::HandleKeyDownMessage(const zaf::KeyMessage& message) {

    if (message.Key() == zaf::Key::Escape) {
        this->Close();
        return true;
    }

    return false;
}

}