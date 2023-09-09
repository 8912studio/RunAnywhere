#pragma once

#include "help/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class BoldParser : public ElementParser {
public:
    static ElementParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

private:
    BoldParser() = default;
};

}