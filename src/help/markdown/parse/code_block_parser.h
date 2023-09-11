#pragma once

#include "help/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class CodeBlockParser : public ElementParser {
public:
    static ElementParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

private:
    CodeBlockParser() = default;

    bool ParseHeadingLine(ParseContext& context, std::size_t& backquote_count) const;
    bool ParseTailingLine(ParseContext& context, std::size_t heading_backquote_count) const;
};

}