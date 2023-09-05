#pragma once

#include "help/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class InlineCodeParser : public ElementParser {
public:
    static ElementParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

private:
    InlineCodeParser() = default;
};

}