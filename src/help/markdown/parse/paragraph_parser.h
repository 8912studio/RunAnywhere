#pragma once

#include "help/markdown/parse/element_parser.h"

namespace ra::help::markdown::parse {

class ParagraphParser : public ElementParser {
public:
    static ElementParser* Instance();

public:
    std::shared_ptr<element::Element> Parse(ParseContext& context) override;

private:
    class LineInfo {
    public:
        element::ElementList elements;
        std::wstring last_piece;
        bool has_concatenate_mark{};
    };

private:
    ParagraphParser() = default;

    LineInfo ParseOneLine(ParseContext& context);
};

}