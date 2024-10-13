#include "module/ai/gpt/dialog/content/dialog_view.h"
#include <zaf/input/keyboard.h>
#include <zaf/rx/creation.h>

namespace ra::mod::ai::gpt {

ZAF_OBJECT_IMPL(DialogView);

DialogView::DialogView(std::shared_ptr<DialogModel> model) :
    round_list_view_(zaf::Create<RoundListView>(model)),
    model_(model) {

    ZAF_EXPECT(model_);
}


void DialogView::AfterParse() {

    __super::AfterParse();

    InitializeRoundListView();
    InitializeInputEdit();
    InitializeSendButton();
    ResetControlStates();
}


void DialogView::InitializeRoundListView() {
    roundListContainer->SetChildren({ round_list_view_ });
}


void DialogView::InitializeInputEdit() {

    Subscriptions() += inputEdit->TextChangedEvent().Subscribe(
        std::bind(&DialogView::ResetControlStates, this));

    Subscriptions() += inputEdit->RectChangedEvent().Subscribe(
        [this](const zaf::RectChangedInfo& event_info) {

        if (inputEdit->Width() != event_info.PreviousRect().size.width) {
            ResetInputHeight();
        }
    });

    Subscriptions() += inputEdit->KeyDownEvent().Subscribe([this](
        const zaf::KeyDownInfo& event_info) {

        if (event_info.Message().Key() == zaf::Key::Enter && !zaf::Keyboard::IsShiftDown()) {
            StartNewRoundOnPressReturn();
            event_info.MarkAsHandled();
        }
    });
}


void DialogView::InitializeSendButton() {

    sendButton->SetBackgroundImagePicker([](const Control& control) {
    
        std::wstring uri{ L"res:///resource/send" };
        if (!control.IsEnabled()) {
            uri += L"_disabled";
        }
        else if (control.ContainsMouse()) {
            uri += L"_highlight";
        }
        else {
            uri += L"_normal";
        }
        uri += L".png";
        return zaf::Image::FromURI(uri, control.GetDPI());
    });

    Subscriptions() += sendButton->ClickEvent().Subscribe(
        std::bind(&DialogView::StartNewRoundOnPressReturn, this));
}


void DialogView::ResetControlStates() {

    ResetInputHeight();
    ResetSendButtonState();
}


void DialogView::ResetInputHeight() {

    auto line_count = inputEdit->LineCount();
    auto text_height = inputEdit->CalculatePreferredSize(inputEdit->Size()).height;

    auto line_height = std::ceil(text_height / line_count);
    constexpr std::size_t max_line_count = 5;

    auto show_line_count = std::min(line_count, max_line_count);
    auto expected_height = show_line_count * line_height + inputEdit->Margin().Height();
    bottomContainer->SetFixedHeight(expected_height);
}


void DialogView::ResetSendButtonState() {

    sendButton->SetIsEnabled(inputEdit->TextLength() > 0);
}


void DialogView::StartNewRoundOnPressReturn() {

    auto question = inputEdit->Text();
    if (question.empty()) {
        return;
    }

    inputEdit->SetText({});

    StartNewRound(std::move(question));
}


void DialogView::StartNewRound(std::wstring question) {
    model_->CreateRound(std::move(question));
}


void DialogView::Chat(std::wstring question) {

    if (question.empty()) {
        return;
    }

    StartNewRound(std::move(question));
}


void DialogView::SetFocusToInputEdit() {
    inputEdit->SetIsFocused(true);
}


std::wstring DialogView::Subject() const {
    return {};
}


zaf::Observable<zaf::None> DialogView::SubjectUpdatedEvent() const {
    return zaf::rx::Just<zaf::None>({});
}

}