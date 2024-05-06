#include "utility/markdown/parse/body_parser.h"
#include <zaf/base/error/contract_error.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/code_block_parser.h"
#include "utility/markdown/parse/header_parser.h"
#include "utility/markdown/parse/paragraph_parser.h"

namespace ra::utility::markdown::parse {

void BodyParser::ParseOneLine(ParseContext& context) {

    //Each body parser can be used only once.
    ZAF_EXPECT(!is_body_finished_);

    std::size_t initial_index = context.CurrentIndex();

    InnerParseOneLine(context);

    //Parse again if the index didn't advance, it will happend when list item parser finishes a
    //list item, which ends without a fence string.
    if (context.CurrentIndex() == initial_index) {
        InnerParseOneLine(context);
    }

    //Make sure each call advances the context, if not, there must be some error in parsing.
    ZAF_EXPECT(context.CurrentIndex() > initial_index);
}


void BodyParser::InnerParseOneLine(ParseContext& context) {

    BlockParser::Result block_result;
    if (ParseOneBlockLine(context, block_result)) {

        auto paragraph_result = paragraph_parser_.FinishCurrentElement();
        if (paragraph_result.element) {
            elements_.push_back(std::move(paragraph_result.element));
        }

        HandleBlockResult(std::move(block_result));
        is_last_paragraph_finished_ = true;
    }
    else {

        auto status = paragraph_parser_.ParseOneLine(context);
        if (status == ParagraphParser::Status::Finished) {

            auto paragraph_result = paragraph_parser_.FinishCurrentElement();
            if (paragraph_result.element) {
                elements_.push_back(std::move(paragraph_result.element));
            }
            
            is_last_paragraph_finished_ = true;
            empty_line_info_.has_trailing_empty_line = true;
        }
        else {

            is_last_paragraph_finished_ = false;

            if (empty_line_info_.has_trailing_empty_line) {
                empty_line_info_.has_middle_empty_line = true;
            }
            empty_line_info_.has_trailing_empty_line = false;
        }
    }
}


bool BodyParser::ParseOneBlockLine(
    ParseContext& context,
    BlockParser::Result& block_result) {

    if (current_block_parser_) {

        auto status = current_block_parser_->ParseOneLine(context);
        ZAF_EXPECT(status != BlockParser::Status::Failed);

        if (status == BlockParser::Status::Continue) {
            return true;
        }

        //Must be Status::Finished here.
        block_result = current_block_parser_->FinishCurrentElement();
        ZAF_EXPECT(block_result.element);
        current_block_parser_ = nullptr;
        return true;
    }
    else {

        std::vector<BlockParser*> block_parsers = {
            &header_parser_, 
            &code_block_parser_,
            &unordered_list_parser_,
            &ordered_list_parser_,
        };

        for (auto each_parser : block_parsers) {

            auto status = each_parser->ParseOneLine(context);
            if (status == BlockParser::Status::Continue) {
                current_block_parser_ = each_parser;
                return true;
            }

            if (status == BlockParser::Status::Finished) {

                block_result = each_parser->FinishCurrentElement();
                ZAF_EXPECT(block_result.element);
                return true;
            }
        }

        return false;
    }
}


void BodyParser::HandleBlockResult(BlockParser::Result&& block_result) {

    if (!block_result.element) {
        return;
    }

    elements_.push_back(std::move(block_result.element));

    if (block_result.empty_line_info.has_middle_empty_line || 
        empty_line_info_.has_trailing_empty_line) {
        empty_line_info_.has_middle_empty_line = true;
    }

    empty_line_info_.has_trailing_empty_line = 
        block_result.empty_line_info.has_trailing_empty_line;
}


BodyParser::Result BodyParser::Finish() {

    if (current_block_parser_) {
        auto block_result = current_block_parser_->FinishCurrentElement();
        HandleBlockResult(std::move(block_result));
    }
    else {
        auto paragraph_result = paragraph_parser_.FinishCurrentElement();
        if (paragraph_result.element) {
            elements_.push_back(std::move(paragraph_result.element));
        }
    }

    is_body_finished_ = true;

    Result result;
    result.elements = std::move(elements_);
    result.empty_line_info = empty_line_info_;
    return result;
}

}