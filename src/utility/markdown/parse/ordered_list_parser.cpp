#include "utility/markdown/parse/ordered_list_parser.h"
#include <zaf/base/error/check.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/ordered_list_item_parser.h"

namespace ra::utility::markdown::parse {

std::unique_ptr<ListItemParser> OrderedListParser::CreateFirstItemParser(ParseContext& context) {
    return CreateItemParser(context);
}


std::unique_ptr<ListItemParser> OrderedListParser::CreateNonFirstItemParser(
    ParseContext& context) {

    return CreateItemParser(context);
}


std::unique_ptr<ListItemParser> OrderedListParser::CreateItemParser(ParseContext& context) {

    auto item_parser = std::make_unique<OrderedListItemParser>();
    auto status = item_parser->ParseOneLine(context);
    ZAF_EXPECT(status != Status::Finished);
    if (status == Status::Continue) {
        return item_parser;
    }
    return nullptr;
}


std::shared_ptr<element::Element> OrderedListParser::CreateListElement(
    element::ListItemStyle item_style,
    element::ElementList list_items) {

    return element::MakeOrderedList(item_style, std::move(list_items));
}

}