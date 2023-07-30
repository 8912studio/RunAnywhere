#include "main/text_block_window.h"
#include <zaf/base/log.h>
#include <zaf/base/none.h>
#include <zaf/base/string/replace.h>
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

    for (auto each_option : { &useCRLF, &useCR, &useLF }) {
        Subscriptions() += (*each_option)->MouseUpEvent().Subscribe(
            std::bind(&TextBlockWindow::OnLineBreakOptionClick, this, std::placeholders::_1));
    }

    Subscriptions() += textEdit->TextChangedEvent().Subscribe(
        [this](const zaf::TextChangedInfo& event_info) {
    
        //Change all line breaks to the same as the first line once text is changed.
        line_break_info_.all_the_same = true;
        UpdateLineBreakOptions();

        AdjustPositionAndSize();
        RaiseTextChangedEvent();
    });
}


void TextBlockWindow::SetObjectPositionInScreen(const zaf::Point& position) {
    object_position_in_screen_ = position;
}


void TextBlockWindow::SetIsReadOnly(bool read_only) {

    textEdit->SetIsReadOnly(read_only);
    scrollableControl->SetBackgroundColor(
        read_only ? zaf::Color::FromRGB(0xF5F5F5) : zaf::Color::White());

    lineBreakOptions->SetIsEnabled(!read_only);
}


std::wstring TextBlockWindow::GetText() const {

    auto result = textEdit->GetText(
        (line_break_info_.first_line_break == utility::LineBreak::CRLF) ?
        zaf::rich_edit::TextFlag::UseCRLF :
        zaf::rich_edit::TextFlag::Default);

    if (line_break_info_.first_line_break == utility::LineBreak::LF) {
        zaf::Replace(result, L"\r", L"\n");
    }

    return result;
}


void TextBlockWindow::SetText(const std::wstring& text) {
    textEdit->SetText(text);
    line_break_info_ = utility::DeterminateLineBreakInfo(text);
    UpdateLineBreakOptions();
    AdjustPositionAndSize();
}


zaf::Observable<std::shared_ptr<TextBlockWindow>> TextBlockWindow::TextChangedEvent() {
    return text_changed_event_.GetObservable();
}


void TextBlockWindow::AdjustPositionAndSize() {

    constexpr float WindowHorizontalBorder = 2;
    constexpr float WindowVerticalBorder = 2;
    constexpr float MaxContentWidth = 500;
    constexpr std::size_t MinShowLineCount = 2;
    constexpr std::size_t MaxShowLineCount = 10;
    constexpr float MinWindowWidth = 320.f;

    auto edit_size = textEdit->CalculatePreferredSize();
    auto line_count = textEdit->LineCount();

    bool need_horizontal_scroll_bar = edit_size.width > MaxContentWidth;
    scrollableControl->SetAllowHorizontalScroll(need_horizontal_scroll_bar);

    bool need_vertical_scroll_bar = line_count > MaxShowLineCount;
    scrollableControl->SetAllowVerticalScroll(need_vertical_scroll_bar);

    auto scroll_control_padding = scrollableControl->Padding();
    scroll_control_padding.bottom = need_horizontal_scroll_bar ? 0 : scroll_control_padding.left;
    scroll_control_padding.right = need_vertical_scroll_bar ? 0 : scroll_control_padding.left;
    scrollableControl->SetPadding(scroll_control_padding);

    zaf::Size window_size;
    window_size.width =
        WindowHorizontalBorder +
        scrollableControl->Padding().Width() +
        (need_horizontal_scroll_bar ? MaxContentWidth : edit_size.width) +
        (need_vertical_scroll_bar ? scrollableControl->VerticalScrollBar()->Width() : 0);

    window_size.width = std::max(window_size.width, MinWindowWidth);

    auto line_height = (edit_size.height - textEdit->Padding().Height()) / line_count;

    auto shown_line_count =
        need_vertical_scroll_bar ?
        MaxShowLineCount : 
        std::max(MinShowLineCount, line_count);

    auto actual_edit_height =
        line_height * shown_line_count +
        textEdit->Padding().Height();

    window_size.height =
        WindowVerticalBorder +
        lineBreakOptions->Height() + 
        lineBreakOptions->Margin().Height() +
        scrollableControl->Padding().Height() +
        actual_edit_height +
        (need_horizontal_scroll_bar ? scrollableControl->HorizontalScrollBar()->Height() : 0);
    
    zaf::Rect window_rect{ object_position_in_screen_, window_size };
    window_rect.position.y -= window_size.height + 4;
    this->SetRect(window_rect);
}


void TextBlockWindow::UpdateLineBreakOptions() {

    for (auto each_option : { &useCRLF, &useCR, &useLF }) {

        zaf::CheckState check_state{ zaf::CheckState::Unchecked };
        if (GetLineBreakByOption(**each_option) == line_break_info_.first_line_break) {
            check_state = 
                line_break_info_.all_the_same ? 
                zaf::CheckState::Checked : 
                zaf::CheckState::Indeterminate;
        }

        (*each_option)->SetCheckState(check_state);
    }
}


void TextBlockWindow::OnLineBreakOptionClick(const zaf::MouseUpInfo& event_info) {

    auto option = zaf::As<LineBreakOption>(event_info.Source());
    if (!option) {
        return;
    }

    line_break_info_.first_line_break = GetLineBreakByOption(*option);
    line_break_info_.all_the_same = true;
    UpdateLineBreakOptions();
    RaiseTextChangedEvent();
}


utility::LineBreak TextBlockWindow::GetLineBreakByOption(const LineBreakOption& option) const {
    if (&option == &*useCR) {
        return utility::LineBreak::CR;
    }
    if (&option == &*useLF) {
        return utility::LineBreak::LF;
    }
    return utility::LineBreak::CRLF;
}


void TextBlockWindow::RaiseTextChangedEvent() {

    text_changed_event_.GetObserver().OnNext(zaf::As<TextBlockWindow>(shared_from_this()));
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