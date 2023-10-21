#pragma once

#include "utility/markdown/parse/list_item_parser.h"

namespace ra::help::markdown::parse {

class OrderedListItemParser : public ListItemParser {
protected:
    bool ParseItemIdentity(ParseContext& context) override;
};

}