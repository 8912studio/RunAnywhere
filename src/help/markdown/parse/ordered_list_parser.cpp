#include "help/markdown/parse/ordered_list_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/ordered_list_item_parser.h"

namespace ra::help::markdown::parse {

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
    element::ElementList list_items) {

    return element::MakeOrderedList(std::move(list_items));
}

}