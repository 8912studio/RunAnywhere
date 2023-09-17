#include "help/markdown/parse/body_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/code_block_parser.h"
#include "help/markdown/parse/header_parser.h"
#include "help/markdown/parse/paragraph_parser.h"

namespace ra::help::markdown::parse {

void BodyParser::ParseOneLine(ParseContext& context) {

    //Each body parser can be used only once.
    ZAF_EXPECT(!is_finished_);

    std::shared_ptr<element::Element> element;
    if (ParseOneBlockLine(context, element)) {

        auto paragraph = paragraph_parser_.FinishCurrentElement();
        if (paragraph) {
            elements_.push_back(std::move(paragraph));
        }

        if (element) {
            elements_.push_back(std::move(element));
        }
    }
    else {

        auto status = paragraph_parser_.ParseOneLine(context);
        if (status == ParagraphParser::Status::Finished) {

            auto paragraph = paragraph_parser_.FinishCurrentElement();
            if (paragraph) {
                elements_.push_back(std::move(paragraph));
            }
        }
    }
}


bool BodyParser::ParseOneBlockLine(
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
            &code_block_parser_,
            &unordered_list_parser_,
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


element::ElementList BodyParser::Finish() {

    if (current_block_parser_) {
        auto element = current_block_parser_->FinishCurrentElement();
        if (element) {
            elements_.push_back(std::move(element));
        }
    }
    else {
        auto paragraph = paragraph_parser_.FinishCurrentElement();
        if (paragraph) {
            elements_.push_back(std::move(paragraph));
        }
    }

    is_finished_ = true;
    return std::move(elements_);
}

}