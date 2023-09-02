#include "help/markdown/header_parser.h"

namespace ra::help::markdown {

ParseStatus HeaderParser::Parse(ParseContext& context, StyledStringBuilder& style_string_builder) {

    if (!context.IsAtLineStart()) {
        return ParseStatus::Mismatched;
    }

    auto reader = context.BeginRead();

    std::size_t hash_count{};
    while (reader.GetChar() == L'#') {
        ++hash_count;
        reader.Forward();
    }

    if (hash_count == 0 || hash_count > 3) {
        reader.Discard();
        return ParseStatus::Mismatched;
    }

    if (reader.GetChar() != L' ') {
        reader.Discard();
        return ParseStatus::Mismatched;
    }

    reader.Forward();

    if (reader.IsAtLineEnd()) {
        reader.Discard();
        return ParseStatus::Mismatched;
    }

    style_string_builder.BeginNewStyle(StyleType::Header1);

    do {
        style_string_builder.Append(reader.GetChar());
        reader.Forward();
    } 
    while (!reader.IsAtLineEnd());

    style_string_builder.CommitStyle();
    return ParseStatus::Ok;
}

}