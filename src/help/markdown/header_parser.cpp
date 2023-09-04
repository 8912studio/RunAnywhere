#include "help/markdown/header_parser.h"

namespace ra::help::markdown {

StyleParser* HeaderParser::Instance() {
    static HeaderParser instance;
    return &instance;
}


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

    StyleType style_type = StyleType(std::size_t(StyleType::Header1) + hash_count - 1);
    style_string_builder.BeginNewStyle(style_type);

    do {
        style_string_builder.Append(reader.GetChar());
        reader.Forward();
    } 
    while (!reader.IsAtLineEnd());

    style_string_builder.CommitStyle();
    return ParseStatus::Ok;
}

}