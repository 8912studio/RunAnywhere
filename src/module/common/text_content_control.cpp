#include "module/common/text_content_control.h"
#include <zaf/base/container/utility/erase.h>
#include <zaf/base/log.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>
#include "utility/text_utility.h"

namespace ra::mod {
namespace {

constexpr float MultiLineFontSize = 16;
constexpr float SingleLineMinFontSize = MultiLineFontSize;
constexpr float SingleLineMaxFontSize = 26;
constexpr std::size_t SingleLineMaxCalculateLength = 100;
constexpr float MinTextLayoutHeight = 80;

}

ZAF_DEFINE_TYPE(TextContentControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/common/text_content_control.xaml")
ZAF_DEFINE_TYPE_END;

void TextContentControl::AfterParse() {

    __super::AfterParse();

    richEdit->SetOLECallback(zaf::As<zaf::rich_edit::OLECallback>(shared_from_this()));
}


void TextContentControl::SetDisplayMode(TextDisplayMode mode) {
    display_mode_ = mode;
}


void TextContentControl::SetText(std::wstring text) {

    text_ = std::move(text);
    has_line_break_ = utility::HasLineBreak(text_);

    auto update_guard = richEdit->BeginUpdate();

    auto font = richEdit->Font();
    font.family_name = display_mode_ == TextDisplayMode::Base64 ? L"Consolas" : L"";
    richEdit->SetFont(font);

    CalculateAndAdjustControls();
}


std::wstring TextContentControl::GetText() const {
    return InnerGetText(zaf::Range{ 0, richEdit->TextLength() });
}


void TextContentControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    CalculateAndAdjustControls();
}


void TextContentControl::CalculateAndAdjustControls() {

    //Ignore abnormal size.
    if (GetTextLayoutWidth() <= 0) {
        return;
    }

    bool has_set_text{};
    if (!has_line_break_) {

        //Try to use single line mode if there is no line break in text.
        if (TryToAdjustForSingleLineText()) {
            return;
        }

        if (display_mode_ == TextDisplayMode::Base64) {
            BreakTextForBase64Mode();
            has_set_text = true;
        }
    }

    //There are line breaks in text, or wrapping is set, use multi line mode.
    AdjustForMultiLineText(has_set_text);
}


bool TextContentControl::TryToAdjustForSingleLineText() {

    //Set rich edit to single line mode first.
    richEdit->SetIsMultiline(false);

    bool has_set_text{};
    bool can_fit_in_single_line = DeterminateIfAllTextCanFitInSingleLine(has_set_text);

    //We use single line if:
    //1. All text can fit in single line.
    //2. Or display mode is set to normal, in such case horizontal scroll bar is shown.
    bool use_single_line = can_fit_in_single_line || display_mode_ == TextDisplayMode::Normal;
    if (!use_single_line) {
        return false;
    }

    if (!has_set_text) {
        richEdit->SetFontSize(SingleLineMinFontSize);
        richEdit->SetText(text_);
    }

    richEdit->SetTextAlignment(zaf::TextAlignment::Center);

    SetControlHeight(MinTextLayoutHeight);
    return true;
}


bool TextContentControl::DeterminateIfAllTextCanFitInSingleLine(bool& has_set_text) {

    //We cannot use single line mode if the text is too long.
    if (text_.length() > SingleLineMaxCalculateLength) {
        return false;
    }

    has_set_text = true;

    //Find suitable font size.
    richEdit->SetText(text_);

    float layout_width = GetTextLayoutWidth();

    for (float font_size = SingleLineMaxFontSize;
        font_size >= SingleLineMinFontSize;
        --font_size) {

        richEdit->SetFontSize(font_size);
        auto edit_size = richEdit->CalculatePreferredSize();

        if (edit_size.width <= layout_width) {
            return true;
        }
    }

    return false;
}


float TextContentControl::GetTextLayoutWidth() const {

    auto content_size = this->ContentSize();
    auto width = content_size.width - scrollControl->VerticalScrollBar()->Width();
    return std::max(width, 0.f);
}


void TextContentControl::BreakTextForBase64Mode() {

    auto calculated_text = text_.substr(0, SingleLineMaxCalculateLength);

    richEdit->SetFontSize(SingleLineMinFontSize);
    richEdit->SetText(calculated_text);

    auto preferred_size = richEdit->CalculatePreferredSize();
    preferred_size.width -= richEdit->Padding().Width();
    preferred_size.width -= richEdit->Border().Width();
    float average_char_width = preferred_size.width / calculated_text.length();

    auto line_length =
        static_cast<std::size_t>(std::floor(GetTextLayoutWidth() / average_char_width));

    std::wstring new_text;
    new_text.reserve(text_.length() + (text_.length() / line_length * 2));
    for (auto index : zaf::Range(0, text_.length())) {

        if (index != 0 && (index % line_length == 0)) {
            new_text += L"\r\n";
        }

        new_text += text_[index];
    }

    richEdit->SetIsMultiline(true);
    richEdit->SetText(new_text);
}


void TextContentControl::AdjustForMultiLineText(bool has_set_text) {

    if (!has_set_text) {
        richEdit->SetText(text_);
    }

    richEdit->SetIsMultiline(true);
    richEdit->SetTextAlignment(zaf::TextAlignment::Left);
    richEdit->SetFontSize(MultiLineFontSize);

    bool need_vertical_scroll{};
    auto edit_height = CalculateRequriedHeightForMultiLineEdit(
        richEdit->CalculatePreferredSize(),
        richEdit->LineCount(),
        zaf::Size{ GetTextLayoutWidth(), MinTextLayoutHeight },
        need_vertical_scroll);

    //Rich edit might show scroll bar incorrectly even if there is enough space to show text,
    //so we have to disable the scroll bar manually.
    scrollControl->SetAllowVerticalScroll(need_vertical_scroll);
    SetControlHeight(edit_height);
}


void TextContentControl::SetControlHeight(float text_layout_height) {

    auto control_height = text_layout_height + scrollControl->HorizontalScrollBar()->Height();
    this->SetFixedHeight(control_height);
}


float TextContentControl::CalculateRequriedHeightForMultiLineEdit(
    const zaf::Size& text_preferrence_size,
    std::size_t line_count,
    const zaf::Size& text_bounds,
    bool& need_vertical_scroll) {

    if (text_preferrence_size.height <= text_bounds.height) {
        need_vertical_scroll = false;
        return text_bounds.height;
    }

    constexpr std::size_t MaxShowLineCount = 10;

    std::size_t show_line_count{};
    if (line_count > MaxShowLineCount) {
        need_vertical_scroll = true;
        show_line_count = MaxShowLineCount;
    }
    else {
        need_vertical_scroll = false;
        show_line_count = line_count;
    }

    auto line_height = text_preferrence_size.height / line_count;
    return show_line_count * line_height;
}


zaf::rich_edit::OperationResult TextContentControl::GetClipboardData(
    zaf::rich_edit::ClipboardOperation operation,
    const zaf::Range& text_range,
    zaf::clipboard::DataObject& data_object) {

    auto text = InnerGetText(text_range);
    data_object.SetText(std::move(text));
    return zaf::rich_edit::OperationResult::OK;
}


std::wstring TextContentControl::InnerGetText(const zaf::Range& text_range) const {

    auto text = richEdit->GetTextInRange(text_range);
    if (display_mode_ == TextDisplayMode::Base64) {
        zaf::Erase(text, L'\r');
    }
    return text;
}

}