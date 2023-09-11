#include "help/markdown/parse/italics_parser.h"
#include "help/markdown/parse/bold_parser.h"
#include "help/markdown/parse/inline_code_parser.h"

namespace ra::help::markdown::parse {

ElementParser* ItalicsParser::Instance() {
    static ItalicsParser instance;
    return &instance;
}


ItalicsParser::Options ItalicsParser::GetOptions() {
    Options result;
    result.element_type = element::ElementType::Italics;
    result.identity_char = L'*';
    result.identity_char_count = 1;
    result.children_parsers = {
        BoldParser::Instance(),
        InlineCodeParser::Instance(),
    };
    return result;
}


ItalicsParser::ItalicsParser() : EmphasisParser(GetOptions()) {

}

}