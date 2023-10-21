#pragma once

#include <optional>
#include "utility/markdown/parse/list_parser.h"

namespace ra::help::markdown::parse {

class UnorderedListParser : public ListParser {
protected:
    std::unique_ptr<ListItemParser> CreateFirstItemParser(ParseContext& context) override;
    std::unique_ptr<ListItemParser> CreateNonFirstItemParser(ParseContext& context) override;
    std::shared_ptr<element::Element> CreateListElement(
        element::ListItemStyle item_style,
        element::ElementList list_items) override;

private:
    wchar_t identity_char_{};
};

}