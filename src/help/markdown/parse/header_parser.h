#pragma once

#include "help/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class HeaderParser : public ElementParser {
public:
    static HeaderParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

private:
    HeaderParser() = default;
};

}