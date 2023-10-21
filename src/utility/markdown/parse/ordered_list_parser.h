#pragma once

#include "utility/markdown/parse/list_parser.h"

namespace ra::utility::markdown::parse {

class OrderedListParser : public ListParser {
protected:
    std::unique_ptr<ListItemParser> CreateFirstItemParser(ParseContext& context) override;
    std::unique_ptr<ListItemParser> CreateNonFirstItemParser(ParseContext& context) override;
    std::shared_ptr<element::Element> CreateListElement(
        element::ListItemStyle item_style,
        element::ElementList list_items) override;

private:
    std::unique_ptr<ListItemParser> CreateItemParser(ParseContext& context);
};

}