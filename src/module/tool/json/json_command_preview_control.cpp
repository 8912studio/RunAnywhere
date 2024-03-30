#include "module/tool/json/json_command_preview_control.h"
#include <format>
#include <boost/json.hpp>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>
#include "module/common/error_messages.h"
#include "module/common/style_constants.h"

namespace ra::mod::tool::json {

ZAF_DEFINE_TYPE(JSONCommandPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/json/json_command_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void JSONCommandPreviewControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    if (error_line_info_) {
        ResetParseError();
    }
    ResetScrollControlHeight();
}


void JSONCommandPreviewControl::ShowResult(const JSONCommandParseResult& result) {

    if (auto styled_text = result.StyledText()) {

        contentView->SetIsVisible(true);

        parsed_json_ = styled_text->Clone();
        ResetParsedJSON();
    }
    else if (auto error = result.Error()) {

        //As boost::json doesn't expose an interface to get its error category, we have to make 
        //an error code so that we can get the error category.
        auto json_error_code = boost::json::make_error_code(boost::json::error::exception);
        if (error->code.category() == json_error_code.category()) {
            ShowParseError(*error);
        }
        else {
            ShowGenericError(*error);
        }
    }
}


void JSONCommandPreviewControl::ShowParseError(const JSONCommandParseResult::ErrorInfo& error) {

    contentView->SetIsVisible(true);
    parseErrorView->SetIsVisible(true);

    error_line_info_ = GetAdjacentLinesAtErrorIndex(
        error.original_text, 
        error.last_parsed_index);

    parseErrorLabel->SetText(std::format(
        L"Parse error at line {} char {}",
        error_line_info_->error_line_index + 1,
        error_line_info_->error_char_index + 1));

    ResetParseError();
}


void JSONCommandPreviewControl::ShowGenericError(const JSONCommandParseResult::ErrorInfo& error) {

    genericErrorView->SetIsVisible(true);
    genericErrorView->ShowHintText(ErrorMessages::NoContentToDisplay);
}


void JSONCommandPreviewControl::OnStyleChanged() {

    genericErrorView->ChangeStyle(Style());

    if (error_line_info_) {
        parseErrorLabel->SetFontSize(BodyFontSize());
        ResetParseError();
    }
    else {
        ResetParsedJSON();
    }

    ResetScrollControlHeight();
}


void JSONCommandPreviewControl::ResetParsedJSON() {

    float font_size = BodyFontSize();

    for (auto& each_font : parsed_json_.RangedFonts()) {
        each_font.Font().size = font_size;
    }

    textBox->SetStyledText(parsed_json_.Clone());
}


void JSONCommandPreviewControl::ResetParseError() {

    zaf::Font font = zaf::Font::Default();
    font.family_name = L"Consolas";
    font.size = BodyFontSize();

    textBox->SetFont(font);
    textBox->SetText(L"H");
    auto char_width = textBox->CalculatePreferredSize().width;
    auto max_length = static_cast<std::size_t>(textBox->Width() / char_width);

    auto error_content = GetShownErrorContent(*error_line_info_, max_length);
    auto error_text = GenerateParseErrorText(error_content, font);
    textBox->SetStyledText(std::move(error_text));

    if (Style() != CommandDisplayStyle::Preserved) {
        textBox->SetPadding(zaf::Frame{ 0, 10, 0, 0 });
    }
    else {
        textBox->SetPadding(zaf::Frame{});
    }
}


void JSONCommandPreviewControl::ResetScrollControlHeight() {

    if (!contentView->IsVisible()) {
        return;
    }

    if (this->Width() <= 0) {
        return;
    }

    auto content_height = textBox->CalculatePreferredSize(textBox->ContentSize()).height;
    float bottom_margin{};

    if (Style() != CommandDisplayStyle::Preserved) {

        constexpr int max_line_count = 10;

        auto line_height = content_height / textBox->LineCount();
        auto max_height = line_height * max_line_count;

        float min_height = 80;
        if (parseErrorView->IsVisible()) {
            min_height -= parseErrorView->Height();
        }

        content_height = std::min(content_height, max_height);
        content_height = std::max(content_height, min_height);
        bottom_margin = 10;
    }

    scrollControl->SetFixedHeight(content_height);
    scrollControl->SetMargin(zaf::Frame{ 0, 0, 0, bottom_margin });
}


float JSONCommandPreviewControl::BodyFontSize() const {
    return Style() ==
        CommandDisplayStyle::Preserved ?
        StyleConstants::PreservedBodyFontSize :
        StyleConstants::NormalBodyMinFontSize;
}


zaf::Frame JSONCommandPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.right = 0;
    return result;
}


JSONCommandPreviewControl::ErrorLineInfo JSONCommandPreviewControl::GetAdjacentLinesAtErrorIndex(
    std::wstring_view text, 
    std::size_t error_index) {

    if (text.empty()) {
        return {};
    }

    std::vector<zaf::Range> line_ranges;
    std::size_t current_line_start_index{};
    std::optional<std::size_t> error_line_index;

    std::size_t char_index = 0;
    while (char_index < text.length()) {

        if (error_index == char_index) {
            error_line_index = line_ranges.size();
        }

        auto ch = text[char_index];
        if (ch == L'\n' || ch == L'\r') {

            line_ranges.emplace_back(
                current_line_start_index,
                char_index - current_line_start_index);

            ++char_index;

            if (ch == L'\r' && text[char_index] == L'\n') {
                if (error_index == char_index) {
                    error_line_index = line_ranges.size() - 1;
                }
                ++char_index;
            }

            current_line_start_index = char_index;
        }
        else {
            ++char_index;
        }
    }
    line_ranges.emplace_back(current_line_start_index, char_index - current_line_start_index);

    if (!error_line_index) {
        error_line_index = line_ranges.size() - 1;
    }

    constexpr std::size_t adjacent_line_count = 3;
    std::size_t begin_line_index =
        *error_line_index >= adjacent_line_count ?
        *error_line_index - adjacent_line_count :
        0;

    std::size_t end_line_index = *error_line_index + adjacent_line_count + 1;
    if (end_line_index > line_ranges.size()) {
        end_line_index = line_ranges.size();
    }

    ErrorLineInfo result;
    for (auto line_index = begin_line_index; line_index < end_line_index; ++line_index) {
        const auto& line_range = line_ranges[line_index];
        auto line = text.substr(line_range.index, line_range.length);
        result.lines.emplace_back(line);
    }

    result.error_line_index = *error_line_index - begin_line_index;

    const auto& error_line_range = line_ranges[*error_line_index];
    result.error_char_index = error_index - error_line_range.index;
    if (result.error_char_index > error_line_range.length) {
        result.error_char_index = error_line_range.length;
    }
    return result;
}


JSONCommandPreviewControl::ErrorContent JSONCommandPreviewControl::GetShownErrorContent(
    const ErrorLineInfo& error_info, 
    std::size_t max_char_count) {

    if (max_char_count == 0) {
        return {};
    }

    //The last char index is reserved for the error index that extends beyond the end of the line.
    auto revised_max_char_count = max_char_count - 1;

    const auto& error_line = error_info.lines[error_info.error_line_index];
    if (revised_max_char_count == 0 || error_line.length() <= revised_max_char_count) {
        ErrorContent result;
        result.shown_text = error_line;
        result.error_index = error_info.error_char_index;
        return result;
    }

    const std::size_t adjacent_char_count = (revised_max_char_count - 1) / 2;

    std::size_t end_index = error_info.error_char_index + 1 + adjacent_char_count;
    
    std::size_t begin_index{};
    bool has_more_at_head{};
    if (error_info.error_char_index > adjacent_char_count) {
        has_more_at_head = true;
        begin_index = error_info.error_char_index - adjacent_char_count;
    }
    else {
        begin_index = 0;
        end_index += adjacent_char_count - error_info.error_char_index;
    }

    bool has_more_at_tail{ true };
    if (end_index > error_line.length()) {

        has_more_at_tail = false;

        auto extra_count = end_index - error_line.length();
        end_index = error_line.length();

        if (begin_index >= extra_count) {
            begin_index -= extra_count;
        }
        else {
            begin_index = 0;
            has_more_at_head = false;
        }
    }

    const std::wstring ellipsis(L"...");

    if (has_more_at_head) {
        begin_index = std::min(begin_index + ellipsis.size() - 1, error_info.error_char_index);
    }

    if (has_more_at_tail) {
        end_index = std::max(end_index - (ellipsis.size() - 1), error_info.error_char_index);
    }

    ErrorContent result;
    if (has_more_at_head) {
        result.shown_text.append(ellipsis);
    }

    result.shown_text.append(error_line.substr(begin_index, end_index - begin_index));

    if (has_more_at_tail) {
        result.shown_text.append(ellipsis);
    }

    result.error_index = error_info.error_char_index - begin_index;
    return result;
}


zaf::textual::StyledText JSONCommandPreviewControl::GenerateParseErrorText(
    const ErrorContent& error_info,
    const zaf::Font& font) {

    zaf::textual::StyledText result;
    result.SetDefaultFont(font);

    zaf::textual::TextStyle text_style;
    text_style.SetTextColor(zaf::Color::FromRGB(0xAA3322));
    result.AppendText(error_info.shown_text, text_style);

    zaf::textual::TextStyle mark_style;
    auto mark_font = font;
    mark_font.weight = zaf::FontWeight::Bold;
    mark_style.SetFont(mark_font);
    mark_style.SetTextColor(zaf::Color::Black());

    result.AppendText(
        std::format(L"\n{}^", std::wstring(error_info.error_index, L' ')),
        mark_style);

    return result;
}

}