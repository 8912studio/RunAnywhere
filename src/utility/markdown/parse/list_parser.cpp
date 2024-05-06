#include "utility/markdown/parse/list_parser.h" 
#include <zaf/base/error/contract_error.h>

namespace ra::utility::markdown::parse {

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


ListParser::Result ListParser::FinishCurrentElement() {

    ZAF_EXPECT(state_.has_value());

    if (state_->current_item_parser) {
        FinishCurrentItem();
    }

    auto item_style =
        state_->empty_line_info.has_middle_empty_line ?
        element::ListItemStyle::Blocks :
        element::ListItemStyle::Lines;

    Result list_result;
    list_result.element = CreateListElement(item_style, std::move(state_->parsed_items));
    list_result.empty_line_info = state_->empty_line_info;

    state_.reset();
    return list_result;
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
        FinishCurrentItem();
    }

    auto next_item_parser = CreateNonFirstItemParser(context);
    if (!next_item_parser) {
        return false;
    }

    state_->current_item_parser = std::move(next_item_parser);
    return true;
}


void ListParser::FinishCurrentItem() {

    auto item_result = state_->current_item_parser->FinishCurrentElement();
    ZAF_EXPECT(item_result.element);

    state_->parsed_items.push_back(std::move(item_result.element));

    if (item_result.empty_line_info.has_middle_empty_line) {
        state_->empty_line_info.has_middle_empty_line = true;
    }
    else if (state_->empty_line_info.has_trailing_empty_line) {
        state_->empty_line_info.has_middle_empty_line = true;
    }

    state_->empty_line_info.has_trailing_empty_line = 
        item_result.empty_line_info.has_trailing_empty_line;

    state_->current_item_parser.reset();
}

}