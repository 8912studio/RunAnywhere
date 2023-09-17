#include "help/markdown/parse/list_item_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/body_parser.h"

namespace ra::help::markdown::parse {

ListItemParser::Status ListItemParser::ParseOneLine(ParseContext& context) {

    if (!state_) {

        if (!ParseItemIdentity(context)) {
            return Status::Failed;
        }

        state_.emplace();
        state_->body_parser->ParseOneLine(context);
        return Status::Continue;
    }
    else {

        if (ParseItemBody(context)) {
            return Status::Continue;
        }
        return Status::Finished;
    }
}


bool ListItemParser::ParseItemBody(ParseContext& context) {

    auto transaction = context.BeginTransaction();

    std::size_t space_count{};
    while (context.CurrentChar() == L' ') {
        ++space_count;
        context.Forward();
    }

    bool is_current_item_finished = space_count < 4 && !context.IsAtLineEnd();
    if (is_current_item_finished) {
        return false;
    }

    state_->body_parser->ParseOneLine(context);

    transaction.Commit();
    return true;
}


std::shared_ptr<element::Element> ListItemParser::FinishCurrentElement() {

    ZAF_EXPECT(state_.has_value());

    auto result = MakeListItem(state_->body_parser->Finish());
    state_.reset();
    return result;
}


ListItemParser::State::State() : body_parser(std::make_unique<BodyParser>()) {

}

ListItemParser::State::~State() {

}

}