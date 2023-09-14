#include "help/markdown/parse/body_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/block_element_parser.h"
#include "help/markdown/parse/paragraph_parser.h"

namespace ra::help::markdown::parse {

BodyParser* BodyParser::Instance() {
    static BodyParser instance;
    return &instance;
}


element::ElementList BodyParser::Parse(ParseContext& context) {

    ParagraphParser paragraph_parser;

    element::ElementList elements;

    while (!context.IsEnd()) {

        //Context must be at line start each time when parsing a block element.
        ZAF_EXPECT(context.IsAtLineStart());

        auto element = BlockElementParser::Instance()->Parse(context);
        if (element) {

            auto paragraph = paragraph_parser.FinishParagraph();
            if (paragraph) {
                elements.push_back(std::move(paragraph));
            }

            elements.push_back(std::move(element));
        }
        else {

            auto paragraph = paragraph_parser.ParseOneLine(context);
            if (paragraph) {
                elements.push_back(std::move(paragraph));
            }
        }
    }

    auto paragraph = paragraph_parser.FinishParagraph();
    if (paragraph) {
        elements.push_back(std::move(paragraph));
    }

    return elements;
}


void BodyParser::ParseEmptyLines(ParseContext& context) {

    while (ParseEmptyLine(context)) {}
}


bool BodyParser::ParseEmptyLine(ParseContext& context) {

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