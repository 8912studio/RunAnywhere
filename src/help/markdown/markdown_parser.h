#pragma once

#include <optional>
#include <string_view>
#include "help/markdown/style_parser.h"
#include "help/markdown/styled_string.h"

namespace ra::help::markdown {

class MarkdownParser {
public:
    StyledString Parse(std::wstring_view input);

private:
    std::optional<std::pair<std::wstring, StyleSpan>> ParseNextStyle(ParseContext& context);
};

}