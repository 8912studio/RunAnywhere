#pragma once

#include "utility/markdown/parse/list_item_parser.h"

namespace ra::utility::markdown::parse {

class OrderedListItemParser : public ListItemParser {
protected:
    bool ParseItemIdentity(ParseContext& context) override;
};

}