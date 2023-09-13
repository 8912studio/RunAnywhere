#pragma once

#include "help/markdown/element/element_type.h"
#include "help/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class ListItemParser : public ElementParser {
public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

protected:
    explicit ListItemParser(element::ElementType element_type);

    virtual bool ParseIdentity(ParseContext& context) = 0;

private:
    bool ParseHeadingIdentities(ParseContext& context);

private:
    element::ElementType element_type_{};
};

}