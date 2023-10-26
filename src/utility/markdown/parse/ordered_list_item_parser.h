#pragma once

#include "utility/markdown/parse/list_item_parser.h"

namespace ra::utility::markdown::parse {

class OrderedListItemParser : public ListItemParser {
public:
    std::size_t Number() const {
        return number_;
    }

protected:
    bool ParseItemIdentity(ParseContext& context) override;

private:
    std::size_t number_{};
};

}