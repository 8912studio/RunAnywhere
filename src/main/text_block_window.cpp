#include "main/text_block_window.h"
#include <zaf/base/log.h>
#include <zaf/base/none.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/graphic/dpi.h>
#include <zaf/object/type_definition.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/creation.h>

namespace ra {

ZAF_DEFINE_TYPE(TextBlockWindow)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///main/text_block_window.xaml")
ZAF_DEFINE_TYPE_END;

TextBlockWindow::TextBlockWindow() {

}


void TextBlockWindow::AfterParse() {

    __super::AfterParse();

    Subscriptions() += textEdit->TextChangedEvent().Subscribe(
        [this](const zaf::TextChangedInfo& event_info) {
    
        AdjustPositionAndSize();

        text_changed_event_.GetObserver().OnNext(zaf::As<TextBlockWindow>(shared_from_this()));
    });
}


void TextBlockWindow::SetObjectPositionInScreen(const zaf::Point& position) {
    object_position_in_screen_ = position;
}


std::wstring TextBlockWindow::GetText() const {
    return textEdit->Text();
}


void TextBlockWindow::SetText(const std::wstring& text) {
    textEdit->SetText(text);
    AdjustPositionAndSize();
}


zaf::Observable<std::shared_ptr<TextBlockWindow>> TextBlockWindow::TextChangedEvent() {
    return text_changed_event_.GetObservable();
}


void TextBlockWindow::AdjustPositionAndSize() {

    constexpr float WindowHorizontalBorder = 2;
    constexpr float WindowVerticalBorder = 2;
    constexpr float MaxContentWidth = 500;
    constexpr int MaxShowLineCount = 10;

    auto edit_size = textEdit->CalculatePreferredSize();
    auto line_count = textEdit->LineCount();

    bool need_horizontal_scroll_bar = edit_size.width > MaxContentWidth;
    scrollableControl->SetAllowHorizontalScroll(need_horizontal_scroll_bar);

    bool need_vertical_scroll_bar = line_count > MaxShowLineCount;
    scrollableControl->SetAllowVerticalScroll(need_vertical_scroll_bar);

    zaf::Size window_size;
    window_size.width =
        WindowHorizontalBorder +
        scrollableControl->Padding().Width() +
        (need_horizontal_scroll_bar ? MaxContentWidth : edit_size.width) +
        (need_vertical_scroll_bar ? scrollableControl->VerticalScrollBar()->Width() : 0);

    auto line_height = (edit_size.height - textEdit->Padding().Height()) / line_count;

    auto actual_edit_height =
        line_height * (need_vertical_scroll_bar ? MaxShowLineCount : line_count) + 
        textEdit->Padding().Height();

    window_size.height =
        WindowVerticalBorder +
        scrollableControl->Padding().Height() +
        actual_edit_height +
        (need_horizontal_scroll_bar ? scrollableControl->HorizontalScrollBar()->Height() : 0);

    constexpr float MinWindowHeight = 40.f;
    window_size.height = std::max(window_size.height, MinWindowHeight);

    zaf::Rect window_rect{ object_position_in_screen_, window_size };
    window_rect.position.y -= window_size.height + 4;
    this->SetRect(window_rect);
}


void TextBlockWindow::OnDeactivated(const zaf::DeactivatedInfo& event_info) {

    __super::OnDeactivated(event_info);

    //Close window at next message loop to avoid focus issues.
    //TODO: Need more elegant method to schedule task at next message loop.
    Subscriptions() += zaf::rx::Create<zaf::None>([this](zaf::Observer<zaf::None>) {
        this->Close();
        return zaf::Subscription{};
    })
    .SubscribeOn(zaf::Scheduler::Main())
    .Subscribe();
}


void TextBlockWindow::OnMessageReceived(const zaf::MessageReceivedInfo& event_info) {

    switch (event_info.Message().ID()) {
        case WM_KEYDOWN: {
            if (zaf::KeyMessage(event_info.Message()).VirtualKey() == VK_ESCAPE) {
                this->Close();
                event_info.MarkAsHandled(0);
            }
            break;
        }
        default:
            break;
    }

    __super::OnMessageReceived(event_info);
}


void TextBlockWindow::OnShow(const zaf::ShowInfo& event_info) {

    __super::OnShow(event_info);

    SetFocus(this->Handle());
    textEdit->SetIsFocused(true);
}

}