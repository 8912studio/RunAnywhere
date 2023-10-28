#include "utility/markdown/parse/list_item_parser.h"
#include <zaf/base/error/check.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/body_parser.h"

namespace ra::utility::markdown::parse {

ListItemParser::Status ListItemParser::ParseOneLine(ParseContext& context) {

    if (!state_) {

        std::size_t begin_index = context.CurrentIndex();
        if (!ParseItemIdentity(context)) {
            return Status::Failed;
        }

        state_.emplace();
        state_->indent_char_count = context.CurrentIndex() - begin_index;
        state_->body_parser->ParseOneLine(context);
        return Status::Continue;
    }
    else {

        if (ParseItemBodyLine(context)) {
            return Status::Continue;
        }
        return Status::Finished;
    }
}


bool ListItemParser::ParseItemBodyLine(ParseContext& context) {

    auto transaction = context.BeginTransaction();

    if (InnerParseItemBodyLine(context)) {
        transaction.Commit();
        return true;
    }

    return false;
}


bool ListItemParser::InnerParseItemBodyLine(ParseContext& context) {

    std::size_t space_count{};
    while (context.CurrentChar() == L' ' && space_count < state_->indent_char_count) {
        ++space_count;
        context.Forward();
    }
    
    auto is_item_body_line = [this, space_count, &context]() {
    
        //Indented lines and empty lines belong to item body.
        if (space_count >= state_->indent_char_count || context.IsAtLineEnd()) {
            return true;
        }

        //Lines begin with item identity doesn't belong to item body.
        if (ParseItemIdentity(context)) {
            return false;
        }

        //If last paragraph is not finished, non-indented lines belong to item body as well.
        return !state_->body_parser->IsLastParagraphFinished();
    }();

    if (is_item_body_line) {
        state_->body_parser->ParseOneLine(context);
        return true;
    }

    return false;
}


ListItemParser::Result ListItemParser::FinishCurrentElement() {

    ZAF_EXPECT(state_.has_value());

    auto body_result = state_->body_parser->Finish();

    Result item_result;
    item_result.element = MakeListItem(std::move(body_result.elements));
    item_result.empty_line_info = body_result.empty_line_info;

    state_.reset();
    return item_result;
}


ListItemParser::State::State() : body_parser(std::make_unique<BodyParser>()) {

}

ListItemParser::State::~State() {

}

}