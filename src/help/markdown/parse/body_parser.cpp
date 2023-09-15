#include "help/markdown/parse/body_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/code_block_parser.h"
#include "help/markdown/parse/header_parser.h"
#include "help/markdown/parse/paragraph_parser.h"

namespace ra::help::markdown::parse {

BodyParser* BodyParser::Instance() {
    static BodyParser instance;
    return &instance;
}


element::ElementList BodyParser::Parse(ParseContext& context) {

    element::ElementList elements;
    while (!context.IsEnd()) {

        //Context must be at line start each time when parsing a block element.
        ZAF_EXPECT(context.IsAtLineStart());

        std::shared_ptr<element::Element> element;
        if (ParseBlockOneLine(context, element)) {

            auto paragraph = paragraph_parser_.FinishCurrentElement();
            if (paragraph) {
                elements.push_back(std::move(paragraph));
            }

            if (element) {
                elements.push_back(std::move(element));
            }
        }
        else {

            auto status = paragraph_parser_.ParseOneLine(context);
            if (status == ParagraphParser::Status::Finished) {

                auto paragraph = paragraph_parser_.FinishCurrentElement();
                if (paragraph) {
                    elements.push_back(std::move(paragraph));
                }
            }
        }
    }

    if (current_block_parser_) {
        auto element = current_block_parser_->FinishCurrentElement();
        if (element) {
            elements.push_back(std::move(element));
        }
    }
    else {
        auto paragraph = paragraph_parser_.FinishCurrentElement();
        if (paragraph) {
            elements.push_back(std::move(paragraph));
        }
    }

    return elements;
}


bool BodyParser::ParseBlockOneLine(
    ParseContext& context,
    std::shared_ptr<element::Element>& element) {

    if (current_block_parser_) {

        auto status = current_block_parser_->ParseOneLine(context);
        ZAF_EXPECT(status != BlockParser::Status::Failed);

        if (status == BlockParser::Status::Continue) {
            return true;
        }

        //Must be Status::Finished here.
        element = current_block_parser_->FinishCurrentElement();
        ZAF_EXPECT(element);
        current_block_parser_ = nullptr;
        return true;
    }
    else {

        std::vector<BlockParser*> block_parsers = {
            &header_parser_, 
            &code_block_parser_ 
        };

        for (auto each_parser : block_parsers) {

            auto status = each_parser->ParseOneLine(context);
            if (status == BlockParser::Status::Continue) {
                current_block_parser_ = each_parser;
                return true;
            }

            if (status == BlockParser::Status::Finished) {

                element = each_parser->FinishCurrentElement();
                ZAF_EXPECT(element);
                return true;
            }
        }

        return false;
    }
}

}