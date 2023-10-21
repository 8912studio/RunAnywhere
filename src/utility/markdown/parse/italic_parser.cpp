#include "utility/markdown/parse/italic_parser.h"
#include "utility/markdown/parse/bold_parser.h"
#include "utility/markdown/parse/inline_code_parser.h"

namespace ra::utility::markdown::parse {

ElementParser* ItalicParser::Instance() {
    static ItalicParser instance;
    return &instance;
}


ItalicParser::Options ItalicParser::GetOptions() {
    Options result;
    result.element_type = element::ElementType::Italic;
    result.identity_char = L'*';
    result.identity_char_count = 1;
    result.children_parsers = {
        BoldParser::Instance(),
        InlineCodeParser::Instance(),
    };
    return result;
}


ItalicParser::ItalicParser() : EmphasisParser(GetOptions()) {

}

}