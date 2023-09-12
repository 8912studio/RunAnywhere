#include "help/markdown/parse/markdown_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/block_element_parser.h"

namespace ra::help::markdown::parse {

MarkdownParser* MarkdownParser::Instance() {
    static MarkdownParser instance;
    return &instance;
}


std::shared_ptr<element::Element> MarkdownParser::Parse(std::wstring_view input) {

    element::ElementList elements;

    ParseContext context(input);
    while (!context.IsEnd()) {

        //Context must be at line start each time when parsing a block element.
        ZAF_EXPECT(context.IsAtLineStart());

        ParseEmptyLines(context);

        auto element = BlockElementParser::Instance()->Parse(context);
        if (element) {
            elements.push_back(std::move(element));
        }
    }

    return element::MakeRoot(std::move(elements));
}


void MarkdownParser::ParseEmptyLines(ParseContext& context) const {

    while (ParseEmptyLine(context)) { }
}


bool MarkdownParser::ParseEmptyLine(ParseContext& context) const {

    if (!context.IsAtLineStart()) {
        return false;
    }

    auto transaction = context.BeginTransaction();

    context.SkipSpaces();

    if (!context.IsAtLineEnd()) {
        return false;
    }

    context.Forward();
    transaction.Commit();
    return true;
}

}