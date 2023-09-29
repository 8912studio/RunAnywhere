#include "help/markdown/parse/unordered_list_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/unordered_list_item_parser.h"

namespace ra::help::markdown::parse {

std::unique_ptr<ListItemParser> UnorderedListParser::CreateFirstItemParser(ParseContext& context) {

    auto item_parser = std::make_unique<UnorderedListItemParser>();
    auto status = item_parser->ParseOneLine(context);
    ZAF_EXPECT(status != Status::Finished);
    if (status == Status::Continue) {
        identity_char_ = item_parser->IdentityChar();
        return item_parser;
    }
    return nullptr;
}


std::unique_ptr<ListItemParser> UnorderedListParser::CreateNonFirstItemParser(
    ParseContext& context) {

    ZAF_EXPECT(identity_char_ != 0);
    auto item_parser = std::make_unique<UnorderedListItemParser>(identity_char_);
    auto status = item_parser->ParseOneLine(context);
    ZAF_EXPECT(status != Status::Finished);
    if (status == Status::Continue) {
        return item_parser;
    }
    return nullptr;
}


std::shared_ptr<element::Element> UnorderedListParser::CreateListElement(
    element::ListItemStyle item_style,
    element::ElementList list_items) {

    return element::MakeUnorderedList(item_style, std::move(list_items));
}

}