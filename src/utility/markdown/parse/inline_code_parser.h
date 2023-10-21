#pragma once

#include "utility/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class InlineCodeParser : public ElementParser {
public:
    static ElementParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

private:
    InlineCodeParser() = default;

    bool ParseHeadingBackquotes(ParseContext& context, std::size_t& heading_backquote_count) const;
    void TryToParseContinuousBackquotes(
        ParseContext& context,
        std::size_t& backquote_count) const;
};

}