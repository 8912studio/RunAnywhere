#include "help/markdown/markdown_parser.h"
#include "help/markdown/header_parser.h"
#include "help/markdown/inline_code_parser.h"

namespace ra::help::markdown {

ParseStatus MarkdownParser::Parse(
    ParseContext& context, 
    StyledStringBuilder& style_string_builder) {

    while (context.GetCurrentIndex() < context.GetLength()) {

        bool is_succeeded = ParseNextStyle(context, style_string_builder);
        if (is_succeeded) {
            continue;
        }
        
        style_string_builder.Append(context.GetCurrentChar());
        context.Forward(1);
    }

    return ParseStatus::Ok;
}


bool MarkdownParser::ParseNextStyle(
    ParseContext& context, 
    StyledStringBuilder& style_string_builder) {

    auto parsers = {
        HeaderParser::Instance(),
        InlineCodeParser::Instance(),
    };

    for (auto each_parser : parsers) {

        auto parse_status = each_parser->Parse(context, style_string_builder);
        if (parse_status == ParseStatus::Ok) {
            return true;
        }
    }

    return false;
}

}