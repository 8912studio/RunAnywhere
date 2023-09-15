#pragma once

#include <zaf/base/non_copyable.h>
#include "help/markdown/element/element.h"
#include "Help/markdown/parse/block_parser.h"
#include "help/markdown/parse/code_block_parser.h"
#include "help/markdown/parse/header_parser.h"
#include "help/markdown/parse/paragraph_parser.h"
#include "help/markdown/parse/parse_context.h"

namespace ra::help::markdown::parse {

class BodyParser : zaf::NonCopyableNonMovable {
public:
    static BodyParser* Instance();

public:
    element::ElementList Parse(ParseContext& context);

private:
    BodyParser() = default;

    bool ParseBlockOneLine(ParseContext& context, std::shared_ptr<element::Element>& element);

private:
    ParagraphParser paragraph_parser_;
    HeaderParser header_parser_;
    CodeBlockParser code_block_parser_;

    BlockParser* current_block_parser_{};
};

}