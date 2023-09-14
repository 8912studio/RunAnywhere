#pragma once

#include <memory>
#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"
#include "help/markdown/parse/parse_context.h"

namespace ra::help::markdown::parse {

class ParagraphParser : zaf::NonCopyable {
public:
    std::shared_ptr<element::Element> ParseOneLine(ParseContext& context);
    std::shared_ptr<element::Element> FinishParagraph();

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