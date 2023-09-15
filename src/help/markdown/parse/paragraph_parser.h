#pragma once

#include "help/markdown/parse/block_parser.h"

namespace ra::help::markdown::parse {

class ParagraphParser : public BlockParser {
public:
    Status ParseOneLine(ParseContext& context) override;
    std::shared_ptr<element::Element> FinishCurrentElement() override;

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
    static LineInfo ParseLineInfo(ParseContext& context);
    static void MergeLineInfo(
        LineInfo* prior_line, 
        LineInfo& current_line,
        element::ElementList& elements);

private:
    std::vector<LineInfo> line_infos_;
};

}