#include "help/markdown/parse/bold_parser.h"
#include "help/markdown/parse/inline_code_parser.h"

namespace ra::help::markdown::parse {

ElementParser* BoldParser::Instance() {
    static BoldParser instance;
    return &instance;
}


BoldParser::Options BoldParser::GetOptions() {
    Options result;
    result.element_type = element::ElementType::Bold;
    result.identity_char = L'*';
    result.identity_char_count = 2;
    result.children_parsers = {
        InlineCodeParser::Instance(),
    };
    return result;
}


BoldParser::BoldParser() : EmphasisParser(GetOptions()) {

}

}