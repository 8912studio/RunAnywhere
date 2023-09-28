#pragma once

#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"
#include "Help/markdown/parse/block_parser.h"
#include "help/markdown/parse/code_block_parser.h"
#include "help/markdown/parse/header_parser.h"
#include "help/markdown/parse/ordered_list_parser.h"
#include "help/markdown/parse/paragraph_parser.h"
#include "help/markdown/parse/parse_context.h"
#include "help/markdown/parse/unordered_list_parser.h"

namespace ra::help::markdown::parse {

class BodyParser : zaf::NonCopyableNonMovable {
public:
    void ParseOneLine(ParseContext& context);
    element::ElementList Finish();

    bool IsLastParagraphFinished() const {
        return is_last_paragraph_finished_;
    }

private:
    void InnerParseOneLine(ParseContext& context);
    bool ParseOneBlockLine(ParseContext& context, std::shared_ptr<element::Element>& element);

private:
    ParagraphParser paragraph_parser_;
    HeaderParser header_parser_;
    CodeBlockParser code_block_parser_;
    UnorderedListParser unordered_list_parser_;
    OrderedListParser ordered_list_parser_;

    BlockParser* current_block_parser_{};
    element::ElementList elements_;
    bool is_last_paragraph_finished_{};
    bool is_body_finished_{};
};

}