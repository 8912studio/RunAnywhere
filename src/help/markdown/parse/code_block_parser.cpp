#include "help/markdown/parse/code_block_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"

namespace ra::help::markdown::parse {

CodeBlockParser::Status CodeBlockParser::ParseOneLine(ParseContext& context) {

    if (!state_) {

        std::size_t backquote_count{};
        if (!ParseHeadingLine(context, backquote_count)) {
            return Status::Failed;
        }

        state_.emplace();
        state_->backquote_count = backquote_count;
        return Status::Continue;
    }
    else {

        if (ParseTailingLine(context)) {
            return Status::Finished;
        }

        ParseContentLine(context);
        return Status::Continue;
    }
}


CodeBlockParser::Result CodeBlockParser::FinishCurrentElement() {

    ZAF_EXPECT(state_.has_value());

    if (!state_->content.empty()) {
        //Remove the last line break.
        state_->content.pop_back();
    }

    auto result = element::MakeCodeBlock(std::move(state_->content));
    state_.reset();
    return result;
}


bool CodeBlockParser::ParseHeadingLine(ParseContext& context, std::size_t& backquote_count) {

    auto transaction = context.BeginTransaction();
    context.SkipSpaces();

    backquote_count = 0;
    while (context.CurrentChar() == L'`') {
        ++backquote_count;
        context.Forward();
    }

    if (backquote_count < 3) {
        return false;
    }

    while (context.CurrentChar() != L'`' && !context.IsAtLineEnd()) {
        context.Forward();
    }

    if (context.CurrentChar() == L'`') {
        return false;
    }

    context.Forward();
    transaction.Commit();
    return true;
}


bool CodeBlockParser::ParseTailingLine(ParseContext& context) {

    auto transaction = context.BeginTransaction();

    context.SkipSpaces();

    std::size_t tailing_backquote_count{};
    while (context.CurrentChar() == L'`') {
        ++tailing_backquote_count;
        context.Forward();
    }

    if (tailing_backquote_count < state_->backquote_count) {
        return false;
    }

    if (!context.IsAtLineEnd()) {
        return false;
    }

    //Eat the tailing line break.
    context.Forward();
    transaction.Commit();
    return true;
}


void CodeBlockParser::ParseContentLine(ParseContext& context) {

    while (!context.IsAtLineEnd()) {
        state_->content += context.CurrentChar();
        context.Forward();
    }

    state_->content += L'\n';
    context.Forward();
}

}