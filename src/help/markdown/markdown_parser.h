#pragma once

#include <string_view>
#include "help/markdown/style_parser.h"
#include "help/markdown/styled_string.h"

namespace ra::help::markdown {

class MarkdownParser {
public:
    StyledString Parse(std::wstring_view input);

private:
    bool ParseNextStyle(ParseContext& context, StyledStringBuilder& style_string_builder);
};

}