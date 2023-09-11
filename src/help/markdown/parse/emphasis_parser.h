#pragma once

#include "help/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class EmphasisParser : public ElementParser {
public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

protected:
    class Options {
    public:
        element::ElementType element_type{};
        wchar_t identity_char{};
        std::size_t identity_char_count{};
        std::vector<ElementParser*> children_parsers;
    };

    explicit EmphasisParser(Options options) : options_(std::move(options)) {

    }

private:
    bool ParseIdentityChars(ParseContext& context) const;
    std::shared_ptr<element::Element> ParseNextChildElement(ParseContext& context) const;

private:
    Options options_;
};

}