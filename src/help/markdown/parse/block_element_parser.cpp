#include "help/markdown/parse/block_element_parser.h"
#include "help/markdown/parse/paragraph_parser.h"

namespace ra::help::markdown::parse {

ElementParser* BlockElementParser::Instance() {
    static BlockElementParser instance;
    return &instance;
}


BlockElementParser::BlockElementParser() {

    parsers_.assign({
        ParagraphParser::Instance(),
    });
}


std::shared_ptr<element::Element> BlockElementParser::Parse(ParseContext& context) {

    for (auto each_parser : parsers_) {

        auto element = each_parser->Parse(context);
        if (element) {
            return element;
        }
    }

    return nullptr;
}

}