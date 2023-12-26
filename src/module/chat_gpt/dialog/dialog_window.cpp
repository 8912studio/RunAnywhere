#include "module/chat_gpt/dialog/dialog_window.h"
#include <zaf/base/error/check.h>
#include <zaf/creation.h>
#include <zaf/object/type_definition.h>
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
            RequestAnswer();
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


void DialogWindow::RequestAnswer() {

    auto question = inputEdit->Text();
    inputEdit->SetText({});

    auto round_view = zaf::Create<RoundView>(dialog_);
    round_view->Start(std::move(question));
    
    messageListView->AddChild(round_view);
}

}