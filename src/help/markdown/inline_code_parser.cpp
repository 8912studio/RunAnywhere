#include "help/markdown/inline_code_parser.h"

namespace ra::help::markdown {

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
    while (reader.GetChar() != L'`');

    return ParseStatus::Ok;
}

}