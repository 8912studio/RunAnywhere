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


void StyledStringBuilder::CommitStyle() {

    ZAF_EXPECT(style_stack_.size() > 1);

    auto new_style_info = std::move(style_stack_.back());
    style_stack_.pop_back();

    auto& parent = style_stack_.back();

    new_style_info.style_span.range.index = new_style_info.absolute_index - parent.absolute_index;
    new_style_info.style_span.range.length = string_.length() - new_style_info.absolute_index;

    parent.style_span.interior_spans.push_back(std::move(new_style_info.style_span));
}


StyledString StyledStringBuilder::Build() {

    ZAF_EXPECT(style_stack_.size() == 1);

    auto root_style_info = std::move(style_stack_.back());
    root_style_info.style_span.range.length = string_.length();

    return StyledString{ std::move(string_), std::move(root_style_info.style_span) };
}

}