#include "help/markdown/styled_string_builder.h"
#include <zaf/base/error/check.h>

namespace ra::help::markdown {

StyledStringBuilder::StyledStringBuilder() {

    style_stack_.emplace_back();
}


void StyledStringBuilder::BeginNewStyle(StyleType style_type) {

    NewStyleInfo new_style_info;
    new_style_info.absolute_index = string_.length();
    new_style_info.style_span.style.type = style_type;

    style_stack_.push_back(std::move(new_style_info));
}


void StyledStringBuilder::Append(std::wstring_view string, StyleSpan style_span) {

    ZAF_EXPECT(!style_stack_.empty());
    ZAF_EXPECT(style_span.range.index == 0 && style_span.range.length == string.length());

    auto& current_style_info = style_stack_.back();

    style_span.range.index = string_.length() - current_style_info.absolute_index;
    current_style_info.style_span.interior_spans.push_back(std::move(style_span));

    string_.append(string);
}


void StyledStringBuilder::CommitStyle() {

    ZAF_EXPECT(style_stack_.size() > 1);

    auto new_style_info = std::move(style_stack_.back());
    style_stack_.pop_back();

    auto& parent = style_stack_.back();

    new_style_info.style_span.range.index = new_style_info.absolute_index - parent.absolute_index;
    new_style_info.style_span.range.length = string_.length() - new_style_info.absolute_index;

    parent.style_span.interior_spans.push_back(std::move(new_style_info.style_span));
}


std::pair<std::wstring, StyleSpan> StyledStringBuilder::BuildIntermediate() {

    ZAF_EXPECT(style_stack_.size() == 1);

    auto root_style_info = std::move(style_stack_.back());
    root_style_info.style_span.range.length = string_.length();

    return std::make_pair(std::move(string_), std::move(root_style_info.style_span));
}

}