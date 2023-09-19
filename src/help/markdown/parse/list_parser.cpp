#include "help/markdown/parse/list_parser.h" 
#include <zaf/base/error/check.h>

namespace ra::help::markdown::parse {

ListParser::Status ListParser::ParseOneLine(ParseContext& context) {

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


std::shared_ptr<element::Element> ListParser::FinishCurrentElement() {

    ZAF_EXPECT(state_.has_value());

    if (state_->current_item_parser) {

        auto element = state_->current_item_parser->FinishCurrentElement();
        ZAF_EXPECT(element);
        state_->parsed_items.push_back(std::move(element));
    }

    auto result = CreateListElement(std::move(state_->parsed_items));
    state_.reset();
    return result;
}


bool ListParser::ParseFirstLine(ParseContext& context) {

    auto item_parser = CreateFirstItemParser(context);
    if (!item_parser) {
        return false;
    }

    state_.emplace();
    state_->current_item_parser = std::move(item_parser);
    return true;
}


bool ListParser::ParseNonFirstLine(ParseContext& context) {

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

    auto next_item_parser = CreateNonFirstItemParser(context);
    if (!next_item_parser) {
        return false;
    }

    state_->current_item_parser = std::move(next_item_parser);
    return true;
}

}