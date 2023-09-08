#include "help/markdown/parse/inline_code_parser.h"
#include "help/markdown/element/factory.h"

namespace ra::help::markdown::parse {

ElementParser* InlineCodeParser::Instance() {
    static InlineCodeParser instance;
    return &instance;
}


std::shared_ptr<element::Element> InlineCodeParser::Parse(ParseContext& context) {

    //If the previous char is '`', means that the parsing of inline code prefix is failed.
    //We don't need to continue to parse the inline code.
    auto previous_char = context.GetCharAtOffset(-1);
    if (previous_char && previous_char == L'`') {
        return nullptr;
    }

    auto reader = context.BeginRead();

    std::size_t heading_backquote_count{};
    while (reader.GetChar() == L'`') {
        ++heading_backquote_count;
        reader.Forward();
    }

    if (heading_backquote_count == 0) {
        return nullptr;
    }

    std::wstring content;
    while (reader.GetChar() != L'`' && !reader.IsAtLineEnd()) {
        content.append(1, reader.GetChar());
        reader.Forward();
    }

    std::size_t tailing_backquote_count{};
    while (reader.GetChar() == L'`') {
        ++tailing_backquote_count;
        reader.Forward();
    }

    if (tailing_backquote_count != heading_backquote_count) {
        reader.Discard();
        return nullptr;
    }

    return element::MakeInlineCode(std::move(content));
}

}