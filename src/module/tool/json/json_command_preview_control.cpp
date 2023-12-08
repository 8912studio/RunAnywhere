#include "module/tool/json/json_command_preview_control.h"
#include <format>
#include <boost/json.hpp>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>
#include <zaf/control/scroll_bar.h>
#include <zaf/object/type_definition.h>
#include "module/common/error_messages.h"

using namespace ra::utility::markdown::render;

namespace ra::mod::tool::json {

ZAF_DEFINE_TYPE(JSONCommandPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/json/json_command_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void JSONCommandPreviewControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    ResetScrollControlHeight();
}


void JSONCommandPreviewControl::ShowResult(const JSONCommandParseResult& result) {

    if (auto styled_text = result.StyledText()) {

        contentView->SetIsVisible(true);
        textBox->SetStyledText(*styled_text);
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

    auto error_info = GetAdjacentLinesAtErrorIndex(
        error.original_text, 
        error.last_parsed_index);

    parseErrorLabel->SetText(std::format(
        L"Parse error at line {} char {}",
        error_info.error_line_index + 1,
        error_info.error_char_index + 1));

    auto error_text = GenerateParseErrorText(error_info);
    textBox->SetStyledText(error_text);
    textBox->SetPadding(zaf::Frame{ 0, 10, 0, 0 });
}


void JSONCommandPreviewControl::ShowGenericError(const JSONCommandParseResult::ErrorInfo& error) {

    genericErrorView->SetIsVisible(true);
    genericErrorView->ShowHintText(ErrorMessages::NoContentToDisplay);
}


void JSONCommandPreviewControl::OnStyleChanged() {

    genericErrorView->ChangeStyle(Style());
    ResetScrollControlHeight();
}


void JSONCommandPreviewControl::ResetScrollControlHeight() {

    if (!contentView->IsVisible()) {
        return;
    }

    auto content_height = textBox->CalculatePreferredSize().height;

    if (Style() == CommandDisplayStyle::Preserved) {
        content_height += scrollControl->HorizontalScrollBar()->Height();
    }
    else {

        constexpr int max_line_count = 10;

        auto line_height = content_height / textBox->LineCount();
        auto max_height = line_height * max_line_count;

        float min_height = 80;
        if (parseErrorView->IsVisible()) {
            min_height -= parseErrorView->Height();
        }

        content_height = std::min(content_height, max_height);
        content_height = std::max(content_height, min_height);
    }

    content_height += scrollControl->HorizontalScrollBar()->Height();
    scrollControl->SetFixedHeight(content_height);
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


StyledText JSONCommandPreviewControl::GenerateParseErrorText(const ErrorLineInfo& error_info) {

    const auto& error_line = error_info.lines[error_info.error_line_index];

    constexpr std::size_t adjacent_char_count = 20;

    std::size_t shown_begin_index{};
    bool has_more_at_head{};
    if (error_info.error_char_index > adjacent_char_count) {
        has_more_at_head = true;
        shown_begin_index = error_info.error_char_index - adjacent_char_count;
    }

    std::size_t shown_end_index = error_info.error_char_index + adjacent_char_count + 1;
    bool has_more_at_tail{ true };
    if (shown_end_index > error_line.length()) {
        has_more_at_tail = false;
        shown_end_index = error_line.length();
    }

    const std::wstring ellipsis(L"...");
    StyledText result;
    if (has_more_at_head) {
        result.Append(ellipsis);
    }

    result.Append(error_line.substr(shown_begin_index, shown_end_index - shown_begin_index));

    if (has_more_at_tail) {
        result.Append(ellipsis);
    }

    result.Append(L"\n");

    TextStyle style;
    style.font.family_name = L"Consolas";
    style.font.size = 16;
    style.text_color = zaf::Color::FromRGB(0xAA3322);
    result.AddStyleToPendingText(style);

    std::size_t heading_space_count = error_info.error_char_index - shown_begin_index;
    if (has_more_at_head) {
        heading_space_count += ellipsis.size();
    }

    result.Append(std::wstring(heading_space_count, L' '));
    result.Append(L"^");

    style.text_color = zaf::Color::Black();
    style.font.weight = zaf::FontWeight::Bold;
    result.AddStyleToPendingText(style);

    return result;
}

}