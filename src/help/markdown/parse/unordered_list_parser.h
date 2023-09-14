#pragma once

#include "help/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class UnorderedListParser : public ElementParser {
public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;
};

}