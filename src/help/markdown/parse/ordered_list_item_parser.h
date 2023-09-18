#pragma once

#include "help/markdown/parse/list_item_parser.h"

namespace ra::help::markdown::parse {

class OrderedListItemParser : public ListItemParser {
public:
    bool ParseItemIdentity(ParseContext& context) override;
};

}