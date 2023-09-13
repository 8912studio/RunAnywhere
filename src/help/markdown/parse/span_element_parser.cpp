#include "help/markdown/parse/span_element_parser.h"
#include "help/markdown/parse/bold_parser.h"
#include "help/markdown/parse/inline_code_parser.h"
#include "help/markdown/parse/italic_parser.h"

namespace ra::help::markdown::parse {

ElementParser* SpanElementParser::Instance() {
    static SpanElementParser instance;
    return &instance;
}


SpanElementParser::SpanElementParser() {

    parsers_.assign({
        ItalicParser::Instance(),
        BoldParser::Instance(),
        InlineCodeParser::Instance(),
    });
}


std::shared_ptr<element::Element> SpanElementParser::Parse(ParseContext& context) {

    for (auto each_parser : parsers_) {

        auto element = each_parser->Parse(context);
        if (element) {
            return element;
        }
    }

    return nullptr;
}

}