#include "help/markdown/parse/block_element_parser.h"
#include "help/markdown/parse/code_block_parser.h"
#include "help/markdown/parse/header_parser.h"
#include "help/markdown/parse/paragraph_parser.h"

namespace ra::help::markdown::parse {

ElementParser* BlockElementParser::Instance() {
    static BlockElementParser instance;
    return &instance;
}


BlockElementParser::BlockElementParser() {

    parsers_.assign({
        HeaderParser::Instance(),
        CodeBlockParser::Instance(),
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