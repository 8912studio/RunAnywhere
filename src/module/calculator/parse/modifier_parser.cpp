#include "module/calculator/parse/modifier_parser.h"
#include <cctype>
#include "module/calculator/parse/common_option_set_parser.h"
#include "module/calculator/parse/extend_option_set_parser.h"
#include "module/calculator/parse/literal_parser.h"

namespace calculator {

ModifierParser* ModifierParser::Instance() {

    static ModifierParser instance;
    return &instance;
}


void ModifierParser::InitializeParsers() {

    static LiteralParser delimiter_parser{ L";" };
    AddParsers({ 
        &delimiter_parser, 
        CommonOptionSetParser::Instance(),
        ExtendOptionSetParser::Instance(),
    });

    AddParsers({});
}

}