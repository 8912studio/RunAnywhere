#include "main/input/text_block_window.h"
#include <zaf/base/auto_reset.h>
#include <zaf/base/log.h>
#include <zaf/base/none.h>
#include <zaf/base/string/replace.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/graphic/dpi.h>
#include <zaf/rx/scheduler.h>
#include <zaf/rx/creation.h>

namespace ra::main::input {
namespace {

zaf::textual::LineBreak ToZafLineBreak(utility::LineBreak line_break) {
    switch (line_break) {
    case utility::LineBreak::CRLF:
        return zaf::textual::LineBreak::CRLF;
    case utility::LineBreak::CR:
        return zaf::textual::LineBreak::CR;
    case utility::LineBreak::LF:
        return zaf::textual::LineBreak::LF;
    default:
        return zaf::textual::LineBreak::Unspecific;
    }
}

}

ZAF_OBJECT_IMPL(TextBlockWindow);

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
    
        if (!is_setting_text_) {

            //Change all line breaks to the same as the first line once text is changed.
            if (textEdit->LineBreak() == zaf::textual::LineBreak::Unspecific) {

                auto line_break_info = utility::DeterminateLineBreakInfo(textEdit->Text());
                textEdit->SetLineBreak(ToZafLineBreak(line_break_info.first_line_break));

                line_break_info.all_the_same = true;
                UpdateLineBreakOptions(line_break_info);
                return;
            }
        }

        Relayout();
        RaiseTextChangedEvent();
    });
}


void TextBlockWindow::SetIsReadOnly(bool read_only) {

    textEdit->SetIsEditable(!read_only);
    scrollableControl->SetBackgroundColor(
        read_only ? zaf::Color::FromRGB(0xF5F5F5) : zaf::Color::White());

    lineBreakOptions->SetIsEnabled(!read_only);
}


std::wstring TextBlockWindow::GetText() {
    return textEdit->Text();
}


void TextBlockWindow::SetText(const std::wstring& text) {

    auto line_break_info = utility::DeterminateLineBreakInfo(text);
    if (line_break_info.all_the_same) {
        textEdit->SetLineBreak(ToZafLineBreak(line_break_info.first_line_break));
    }
    else {
        textEdit->SetLineBreak(zaf::textual::LineBreak::Unspecific);
    }

    {
        auto auto_reset = zaf::MakeAutoReset(is_setting_text_, true);
        textEdit->SetText(text);
    }

    UpdateLineBreakOptions(line_break_info);
}


zaf::Size TextBlockWindow::CalculateWindowContentSize() {

    constexpr std::size_t MinShowLineCount = 2;
    constexpr std::size_t MaxShowLineCount = 10;

    auto edit_size = textEdit->CalculatePreferredSize();
    auto line_count = textEdit->LineCount();

    bool need_horizontal_scroll_bar = edit_size.width > MaxWindowContentWidth;
    scrollableControl->SetAllowHorizontalScroll(need_horizontal_scroll_bar);

    bool need_vertical_scroll_bar = line_count > MaxShowLineCount;
    scrollableControl->SetAllowVerticalScroll(need_vertical_scroll_bar);

    auto scroll_control_padding = scrollableControl->Padding();
    scroll_control_padding.bottom = need_horizontal_scroll_bar ? 0 : scroll_control_padding.left;
    scroll_control_padding.right = need_vertical_scroll_bar ? 0 : scroll_control_padding.left;
    scrollableControl->SetPadding(scroll_control_padding);

    float content_width =
        scrollableControl->Padding().Width() +
        (need_horizontal_scroll_bar ? MaxWindowContentWidth : edit_size.width) +
        (need_vertical_scroll_bar ? scrollableControl->VerticalScrollBar()->Width() : 0);

    auto line_height = (edit_size.height - textEdit->Padding().Height()) / line_count;

    auto shown_line_count =
        need_vertical_scroll_bar ?
        MaxShowLineCount : 
        std::max(MinShowLineCount, line_count);

    auto actual_edit_height =
        line_height * shown_line_count +
        textEdit->Padding().Height();

    float content_height =
        lineBreakOptions->Height() +
        lineBreakOptions->Margin().Height() +
        scrollableControl->Padding().Height() +
        actual_edit_height +
        (need_horizontal_scroll_bar ? scrollableControl->HorizontalScrollBar()->Height() : 0);

    return zaf::Size{ content_width, content_height };
}


void TextBlockWindow::UpdateLineBreakOptions(const utility::LineBreakInfo& line_break_info) {

    for (auto each_option : { &useCRLF, &useCR, &useLF }) {

        zaf::CheckState check_state{ zaf::CheckState::Unchecked };
        if (GetLineBreakByOption(**each_option) == line_break_info.first_line_break) {
            check_state = 
                line_break_info.all_the_same ?
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

    auto line_break = GetLineBreakByOption(*option);
    textEdit->SetLineBreak(ToZafLineBreak(line_break));
    textEdit->SetIsFocused(false);

    utility::LineBreakInfo line_break_info;
    line_break_info.first_line_break = line_break;
    line_break_info.all_the_same = true;
    UpdateLineBreakOptions(line_break_info);
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


void TextBlockWindow::OnShow(const zaf::ShowInfo& event_info) {

    __super::OnShow(event_info);

    textEdit->SetIsFocused(true);
}

}