#include "utility/markdown/parse/ordered_list_parser.h"
#include <zaf/base/error/check.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/ordered_list_item_parser.h"

namespace ra::utility::markdown::parse {

std::unique_ptr<ListItemParser> OrderedListParser::CreateFirstItemParser(ParseContext& context) {
    return CreateItemParser(context, true);
}


std::unique_ptr<ListItemParser> OrderedListParser::CreateNonFirstItemParser(
    ParseContext& context) {

    return CreateItemParser(context, false);
}


std::unique_ptr<ListItemParser> OrderedListParser::CreateItemParser(
    ParseContext& context,
    bool is_first) {

    auto item_parser = std::make_unique<OrderedListItemParser>();
    auto status = item_parser->ParseOneLine(context);
    ZAF_EXPECT(status != Status::Finished);

    if (status == Status::Continue) {
        if (is_first) {
            first_number_ = item_parser->Number();
        }
        return item_parser;
    }
    return nullptr;
}


std::shared_ptr<element::Element> OrderedListParser::CreateListElement(
    element::ListItemStyle item_style,
    element::ElementList list_items) {

    return element::MakeOrderedList(item_style, first_number_, std::move(list_items));
}

}