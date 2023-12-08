#include "module/tool/json/json_command_preview_control.h"
#include <boost/json.hpp>
#include <zaf/base/error/basic_error.h>
#include <zaf/base/range.h>
#include <zaf/base/string/join.h>
#include <zaf/object/type_definition.h>

namespace ra::mod::tool::json {

ZAF_DEFINE_TYPE(JSONCommandPreviewControl)
ZAF_DEFINE_TYPE_RESOURCE_URI(L"res:///module/tool/json/json_command_preview_control.xaml")
ZAF_DEFINE_TYPE_END;

void JSONCommandPreviewControl::AfterParse() {

    __super::AfterParse();
}


void JSONCommandPreviewControl::Layout(const zaf::Rect& previous_rect) {

    __super::Layout(previous_rect);

    ResetHeight();
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
    parseErrorLabel->SetText(L"Parse error");

    auto error_lines = GetAdjacentLinesAtErrorIndex(error.original_text, error.last_parsed_index);

    std::wstring error_text;
    for (auto line_index : zaf::Range(0, error_lines.lines.size())) {

        error_text.append(error_lines.lines[line_index]);
        error_text.append(1, L'\n');

        if (line_index == error_lines.error_line_index) {
            error_text.append(error_lines.error_char_index, L' ');
            error_text.append(1, L'^');
            error_text.append(1, L'\n');
        }
    }

    utility::markdown::render::StyledText styled_text;
    styled_text.Append(error_text);

    utility::markdown::render::TextStyle style;
    style.font.family_name = L"Consolas";
    style.font.size = 16;
    styled_text.AddStyleToPendingText(style);

    textBox->SetStyledText(styled_text);
}


void JSONCommandPreviewControl::ShowGenericError(const JSONCommandParseResult::ErrorInfo& error) {

}


void JSONCommandPreviewControl::OnStyleChanged() {
    ResetHeight();
}


void JSONCommandPreviewControl::ResetHeight() {

    if (contentView->IsVisible()) {

        auto content_size = textBox->CalculatePreferredSize();
        if (Style() == CommandDisplayStyle::Preserved) {
            scrollControl->SetFixedHeight(content_size.height);
        }
        else {

            constexpr float min_height = 80;
            constexpr int max_line_count = 10;

            auto line_height = content_size.height / textBox->LineCount();
            auto max_height = line_height * max_line_count;

            auto height = std::min(content_size.height, max_height);
            height = std::max(height, min_height);
            scrollControl->SetFixedHeight(height);
        }
    }
}


zaf::Frame JSONCommandPreviewControl::GetExpectedMargin() {

    auto result = __super::GetExpectedMargin();
    result.right = 0;
    result.top = 4;
    result.bottom = 6;
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

    result.error_line_index = *error_line_index;

    const auto& error_line_range = line_ranges[*error_line_index];
    result.error_char_index = error_index - error_line_range.index;
    if (result.error_char_index > error_line_range.length) {
        result.error_char_index = error_line_range.length;
    }
    return result;
}

}