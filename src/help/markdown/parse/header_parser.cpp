#include "help/markdown/parse/header_parser.h"

namespace ra::help::markdown::parse {

HeaderParser* HeaderParser::Instance() {
    static HeaderParser instance;
    return &instance;
}


std::shared_ptr<element::Element> HeaderParser::Parse(ParseContext& context) {

    /*
    if (!context.IsAtLineStart()) {
        return nullptr;
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

    PlainStringBuilder plain_string_builder;
    plain_string_builder.SetIsFirstPart(true);

    do {
        plain_string_builder.Append(reader.GetChar());
        reader.Forward();
    } while (!reader.IsAtLineEnd());

    auto plain_string = plain_string_builder.Build(true);
    ZAF_EXPECT(!plain_string.empty());

    style_string_builder.Append(plain_string);
    style_string_builder.CommitStyle();
    return ParseStatus::Ok;
    */
}

}