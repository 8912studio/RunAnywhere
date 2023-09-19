#pragma once

#include "help/markdown/parse/list_parser.h"

namespace ra::help::markdown::parse {

class OrderedListParser : public ListParser {
protected:
    std::unique_ptr<ListItemParser> CreateFirstItemParser(ParseContext& context) override;
    std::unique_ptr<ListItemParser> CreateNonFirstItemParser(ParseContext& context) override;
    std::shared_ptr<element::Element> CreateListElement(element::ElementList list_items) override;

private:
    std::unique_ptr<ListItemParser> CreateItemParser(ParseContext& context);
};

}