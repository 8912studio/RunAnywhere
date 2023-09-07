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
        //The first text piece at the start of the line.
        std::wstring heading_text;

        //Parsed elements.
        element::ElementList elements;

        //The last text piece at the end of the line.
        std::wstring tailing_text;

        //Whether there are two or more spaces at the end of line.
        bool has_tailing_spaces{};
    };

private:
    ParagraphParser() = default;

    static LineInfo ParseOneLine(ParseContext& context);
    static void MergeLineInfo(
        LineInfo* prior_line, 
        LineInfo& current_line,
        element::ElementList& elements);
};

}