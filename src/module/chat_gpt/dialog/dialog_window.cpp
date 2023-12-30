#include "module/chat_gpt/dialog/dialog_window.h"
#include <zaf/base/error/check.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/creation.h>
#include <zaf/rx/scheduler.h>
#include "module/chat_gpt/dialog/round_view.h"

namespace ra::mod::chat_gpt {

ZAF_DEFINE_TYPE(DialogWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/chat_gpt/dialog/dialog_window.xaml")
ZAF_DEFINE_TYPE_END;


DialogWindow::DialogWindow(std::shared_ptr<Dialog> dialog) : dialog_(std::move(dialog)) {

    ZAF_EXPECT(dialog_);
}


void DialogWindow::AfterParse() {

    __super::AfterParse();

    ResetInputHeight();

    Subscriptions() += inputEdit->TextChangedEvent().Subscribe(
        std::bind(&DialogWindow::ResetInputHeight, this));

    Subscriptions() += inputEdit->RectChangedEvent().Subscribe(
        [this](const zaf::RectChangedInfo& event_info) {

        if (inputEdit->Width() != event_info.PreviousRect().size.width) {
            ResetInputHeight();
        }
    });

    Subscriptions() += inputEdit->KeyDownEvent().Subscribe([this](
        const zaf::KeyDownInfo& event_info) {

        if (event_info.Message().VirtualKey() == VK_RETURN && (GetKeyState(VK_SHIFT) >> 15) == 0) {
            StartNewRoundOnPressReturn();
            event_info.MarkAsHandled();
        }
    });
}


void DialogWindow::ResetInputHeight() {

    auto line_count = inputEdit->LineCount();
    auto text_height = inputEdit->CalculatePreferredSize(inputEdit->ContentSize()).height;
    text_height -= inputEdit->Padding().Height();

    auto line_height = std::ceil(text_height / line_count);
    constexpr std::size_t max_line_count = 5;

    auto show_line_count = std::min(line_count, max_line_count);
    auto expected_height = show_line_count * line_height + inputEdit->Padding().Height();
    inputContainer->SetFixedHeight(expected_height);
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

    if (message.VirtualKey() == VK_ESCAPE) {
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
        inputEdit->SetIsFocused(true);
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


void DialogWindow::Chat(std::wstring question) {

    if (question.empty()) {
        return;
    }

    StartNewRound(question);
}


void DialogWindow::StartNewRoundOnPressReturn() {

    auto question = inputEdit->Text();
    if (question.empty()) {
        return;
    }

    inputEdit->SetText({});

    StartNewRound(std::move(question));
}


void DialogWindow::StartNewRound(std::wstring question) {

    auto round_view = zaf::Create<RoundView>(dialog_);
    auto finish_observable = round_view->Start(std::move(question));

    roundListView->AddChild(round_view);
    roundScrollControl->ScrollDownToEnd();

    Subscriptions() += finish_observable.Subscribe(std::bind([this, round_view]() {

        auto answer_view_position = round_view->AnswerView()->TranslatePositionToParent({});
        float scroll_to_position = round_view->Y() + answer_view_position.y;

        roundScrollControl->VerticalScrollBar()->SetValue(static_cast<int>(scroll_to_position));
    }));
}

}