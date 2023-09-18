#include "help/markdown/parse/unordered_list_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"

namespace ra::help::markdown::parse {

UnorderedListParser::Status UnorderedListParser::ParseOneLine(ParseContext& context) {

    if (!state_) {
        if (ParseFirstLine(context)) {
            return Status::Continue;
        }
        return Status::Failed;
    }
    else {
        if (ParseNonFirstLine(context)) {
            return Status::Continue;
        }
        return Status::Finished;
    }
}


std::shared_ptr<element::Element> UnorderedListParser::FinishCurrentElement() {

    ZAF_EXPECT(state_.has_value());

    if (state_->current_item_parser) {

        auto element = state_->current_item_parser->FinishCurrentElement();
        ZAF_EXPECT(element);
        state_->parsed_items.push_back(std::move(element));
    }

    auto result = element::MakeUnorderedList(std::move(state_->parsed_items));
    state_.reset();
    return result;
}


bool UnorderedListParser::ParseFirstLine(ParseContext& context) {

    auto item_parser = std::make_unique<UnorderedListItemParser>();
    auto parse_status = item_parser->ParseOneLine(context);
    if (parse_status == Status::Failed) {
        return false;
    }

    ZAF_EXPECT(parse_status == Status::Continue);

    state_.emplace();
    state_->identity_char = item_parser->IdentityChar();
    state_->current_item_parser = std::move(item_parser);
    return true;
}


bool UnorderedListParser::ParseNonFirstLine(ParseContext& context) {

    if (state_->current_item_parser) {

        auto parse_status = state_->current_item_parser->ParseOneLine(context);
        if (parse_status == Status::Continue) {
            return true;
        }

        ZAF_EXPECT(parse_status == Status::Finished);

        auto element = state_->current_item_parser->FinishCurrentElement();
        ZAF_EXPECT(element);

        state_->parsed_items.push_back(std::move(element));
        state_->current_item_parser.reset();
    }

    auto next_item_parser = std::make_unique<UnorderedListItemParser>(state_->identity_char);
    auto parse_status = next_item_parser->ParseOneLine(context);
    ZAF_EXPECT(parse_status != Status::Finished);

    if (parse_status == Status::Failed) {
        return false;
    }

    state_->current_item_parser = std::move(next_item_parser);
    return true;
}

}