#include "utility/markdown/parse/unordered_list_item_parser.h"

namespace ra::help::markdown::parse {

UnorderedListItemParser::UnorderedListItemParser() {

}


UnorderedListItemParser::UnorderedListItemParser(wchar_t identity_char) : 
    identity_char_(identity_char) {

}


bool UnorderedListItemParser::ParseItemIdentity(ParseContext& context) {

    auto transaction = context.BeginTransaction();
    auto ch = context.CurrentChar();

    if (identity_char_) {
        if (ch != *identity_char_) {
            return false;
        }
    }
    else {
        if (ch != L'*' && ch != L'+' && ch != L'-') {
            return false;
        }
        identity_char_ = ch;
    }

    context.Forward();
    if (context.CurrentChar() != L' ') {
        return false;
    }

    context.SkipSpaces();
    if (context.IsAtLineEnd()) {
        return false;
    }
    
    transaction.Commit();
    return true;
}

}