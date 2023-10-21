#pragma once

#include "utility/markdown/parse/element_parser.h"

namespace ra::utility::markdown::parse {

class SpanElementParser : public ElementParser {
public:
    static ElementParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

private:
    SpanElementParser();

private:
    std::vector<ElementParser*> parsers_;
};

}