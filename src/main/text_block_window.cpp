#include "main/text_block_window.h"
#include <zaf/base/log.h>
#include <zaf/base/none.h>
#include <zaf/control/scroll_bar.h>
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
    
        ResizeToSuitableSize();

        text_changed_event_.GetObserver().OnNext(zaf::As<TextBlockWindow>(shared_from_this()));
    });
}


std::wstring TextBlockWindow::GetText() const {
    return textEdit->Text();
}


void TextBlockWindow::SetText(const std::wstring& text) {
    textEdit->SetText(text);
}


zaf::Observable<std::shared_ptr<TextBlockWindow>> TextBlockWindow::TextChangedEvent() {
    return text_changed_event_.GetObservable();
}


void TextBlockWindow::ResizeToSuitableSize() {

    constexpr float WindowHorizontalBorder = 2;
    constexpr float WindowVerticalBorder = 2;
    constexpr float MaxContentWidth = 500;
    constexpr int MaxShowLineCount = 6;

    auto edit_size = textEdit->CalculatePreferredSize();

    auto line_height = 
        (edit_size.height - textEdit->Padding().Height()) / textEdit->LineCount();

    const float max_edit_height = line_height * MaxShowLineCount + textEdit->Padding().Height();

    bool need_horizontal_scroll_bar = edit_size.width > MaxContentWidth;
    scrollableControl->SetAllowHorizontalScroll(need_horizontal_scroll_bar);

    bool need_vertical_scroll_bar = edit_size.height > max_edit_height;
    scrollableControl->SetAllowVerticalScroll(need_vertical_scroll_bar);

    zaf::Size window_size;
    window_size.width =
        WindowHorizontalBorder +
        scrollableControl->Padding().Width() +
        (need_horizontal_scroll_bar ? MaxContentWidth : edit_size.width) +
        (need_vertical_scroll_bar ? scrollableControl->VerticalScrollBar()->Width() : 0);

    window_size.height =
        WindowVerticalBorder +
        scrollableControl->Padding().Height() +
        (need_vertical_scroll_bar ? max_edit_height : edit_size.height) +
        (need_horizontal_scroll_bar ? scrollableControl->HorizontalScrollBar()->Height() : 0);

    this->SetSize(window_size);
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

    if (event_info.Message().ID() == WM_KEYDOWN) {

        if (zaf::KeyMessage(event_info.Message()).VirtualKey() == VK_ESCAPE) {
            this->Close();
            event_info.MarkAsHandled(0);
        }
    }

    __super::OnMessageReceived(event_info);
}

}