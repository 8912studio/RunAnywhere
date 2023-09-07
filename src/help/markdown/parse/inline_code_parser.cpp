#include "help/markdown/parse/inline_code_parser.h"
#include "help/markdown/element/factory.h"

namespace ra::help::markdown::parse {

ElementParser* InlineCodeParser::Instance() {
    static InlineCodeParser instance;
    return &instance;
}


std::shared_ptr<element::Element> InlineCodeParser::Parse(ParseContext& context) {

    if (context.GetCurrentChar() != L'`') {
        return nullptr;
    }

    auto reader = context.BeginRead();
    reader.Forward();

    std::wstring content;
    while (reader.GetChar() != L'`' && !reader.IsAtLineEnd()) {
        content.append(1, reader.GetChar());
        reader.Forward();
    }

    if (content.empty()) {
        reader.Discard();
        return nullptr;
    }

    if (reader.GetChar() != L'`') {
        reader.Discard();
        return nullptr;
    }

    reader.Forward();

    return element::MakeInlineCode(std::move(content));
}

}