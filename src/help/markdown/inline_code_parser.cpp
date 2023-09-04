#include "help/markdown/inline_code_parser.h"

namespace ra::help::markdown {

StyleParser* InlineCodeParser::Instance() {
    static InlineCodeParser instance;
    return &instance;
}


ParseStatus InlineCodeParser::Parse(ParseContext& context, StyledStringBuilder& style_builder) {

    if (context.GetCurrentChar() != L'`') {
        return ParseStatus::Mismatched;
    }

    auto reader = context.BeginRead();
    reader.Forward();

    if (reader.GetChar() == L'`') {
        reader.Discard();
        return ParseStatus::Mismatched;
    }

    style_builder.BeginNewStyle(StyleType::InlineCode);

    do {
        style_builder.Append(reader.GetChar());
        reader.Forward();
    }
    while (reader.GetChar() != L'`' && !reader.IsAtLineEnd());

    if (reader.GetChar() == L'`') {
        reader.Forward();
    }

    style_builder.CommitStyle();

    return ParseStatus::Ok;
}

}