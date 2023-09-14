#include "help/markdown/parse/markdown_parser.h"
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/body_parser.h"

namespace ra::help::markdown::parse {

MarkdownParser* MarkdownParser::Instance() {
    static MarkdownParser instance;
    return &instance;
}


std::shared_ptr<element::Element> MarkdownParser::Parse(std::wstring_view input) {

    ParseContext context(input);
    auto elements = BodyParser::Instance()->Parse(context);
    return element::MakeRoot(std::move(elements));
}

}