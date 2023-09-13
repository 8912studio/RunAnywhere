#include "help/markdown/parse/list_item_parser.h"
#include <zaf/base/error/check.h>

namespace ra::help::markdown::parse {

ListItemParser::ListItemParser(element::ElementType element_type) : element_type_(element_type) {

    ZAF_EXPECT(
        element_type_ == element::ElementType::OrderedListItem ||
        element_type_ == element::ElementType::UnorderedListItem);
}


std::shared_ptr<element::Element> ListItemParser::Parse(ParseContext& context) {

    auto transaction = context.BeginTransaction();

    if (!ParseHeadingIdentities(context)) {
        return nullptr;
    }

    return nullptr;
}


bool ListItemParser::ParseHeadingIdentities(ParseContext& context) {

    if (!context.IsAtLineStart()) {
        return false;
    }

    context.SkipSpaces();

    if (!ParseIdentity(context)) {
        return false;
    }

    if (context.CurrentChar() != L'.') {
        return false;
    }

    context.Forward();

    if (context.CurrentChar() != L' ') {
        return false;
    }

    context.Forward();
    return true;
}

}