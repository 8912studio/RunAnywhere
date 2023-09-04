#include "help/markdown/markdown_parser.h"
#include "help/markdown/header_parser.h"
#include "help/markdown/inline_code_parser.h"
#include "help/markdown/plain_string_builder.h"

namespace ra::help::markdown {

StyledString MarkdownParser::Parse(std::wstring_view input) {

    ParseContext context(input);
    PlainStringBuilder plain_string_builder;
    StyledStringBuilder styled_string_builder;

    while (context.GetCurrentIndex() < context.GetLength()) {

        auto style_intemediate = ParseNextStyle(context);
        if (style_intemediate) {

            auto plain_string = plain_string_builder.Build(false);
            if (!plain_string.empty()) {
                styled_string_builder.Append(plain_string);
            }

            styled_string_builder.Append(
                style_intemediate->first, 
                std::move(style_intemediate->second));
        }
        else {

            plain_string_builder.Append(context.GetCurrentChar());
            context.Forward(1);
        }
    }

    auto plain_string = plain_string_builder.Build(true);
    if (!plain_string.empty()) {
        styled_string_builder.Append(plain_string);
    }

    auto intermediate = styled_string_builder.BuildIntermediate();
    return StyledString(std::move(intermediate.first), std::move(intermediate.second));
}


std::optional<std::pair<std::wstring, StyleSpan>> MarkdownParser::ParseNextStyle(
    ParseContext& context) {

    auto parsers = {
        HeaderParser::Instance(),
        InlineCodeParser::Instance(),
    };

    StyledStringBuilder builder;

    for (auto each_parser : parsers) {

        auto parse_status = each_parser->Parse(context, builder);
        if (parse_status == ParseStatus::Ok) {
            
            auto intermediate = builder.BuildIntermediate();
            ZAF_EXPECT(intermediate.second.interior_spans.size() == 1);

            return std::make_pair(
                std::move(intermediate.first),
                std::move(intermediate.second.interior_spans.front()));
        }
    }

    return std::nullopt;
}

}