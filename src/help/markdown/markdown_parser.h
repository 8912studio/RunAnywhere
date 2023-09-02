#pragma once

#include <string>
#include "help/markdown/style_parser.h"

namespace ra::help::markdown {

class MarkdownParser : public StyleParser {
public:
    ParseStatus Parse(ParseContext& context, StyledStringBuilder& style_string_builder) override;

private:
    bool ParseNextStyle(ParseContext& context, StyledStringBuilder& style_string_builder);
};

}