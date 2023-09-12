#include "help/markdown/parse/markdown_parser.h"
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/block_element_parser.h"

namespace ra::help::markdown::parse {

MarkdownParser* MarkdownParser::Instance() {
    static MarkdownParser instance;
    return &instance;
}


std::shared_ptr<element::Element> MarkdownParser::Parse(std::wstring_view input) {

    element::ElementList elements;

    ParseContext context(input);
    while (true) {

        auto element = BlockElementParser::Instance()->Parse(context);
        if (!element) {
            break;
        }

        elements.push_back(std::move(element));
    }

    return element::MakeRoot(std::move(elements));
}

}