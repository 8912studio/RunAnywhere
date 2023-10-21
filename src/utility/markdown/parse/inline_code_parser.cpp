#include "utility/markdown/parse/inline_code_parser.h"
#include <zaf/base/string/trim.h>
#include "utility/markdown/element/factory.h"

namespace ra::utility::markdown::parse {

ElementParser* InlineCodeParser::Instance() {
    static InlineCodeParser instance;
    return &instance;
}


std::shared_ptr<element::Element> InlineCodeParser::Parse(ParseContext& context) {

    //If the previous char is '`', means that the parsing of inline code prefix is failed.
    //We don't need to continue to parse the inline code.
    auto previous_char = context.PreviousChar();
    if (previous_char && previous_char == L'`') {
        return nullptr;
    }

    auto transaction = context.BeginTransaction();

    std::size_t heading_backquote_count{};
    if (!ParseHeadingBackquotes(context, heading_backquote_count)) {
        return nullptr;
    }

    std::wstring content;
    bool has_tailing_backquotes{};

    while (!context.IsAtLineEnd()) {

        std::size_t backquote_count{};
        TryToParseContinuousBackquotes(context, backquote_count);

        if (backquote_count == 0) {
            content.append(1, context.CurrentChar());
            context.Forward();
        }
        else if (backquote_count != heading_backquote_count) {
            content.append(backquote_count, L'`');
        }
        else {
            has_tailing_backquotes = true;
            break;
        }
    }

    if (!has_tailing_backquotes) {
        return nullptr;
    }

    //Not allow empty content.
    if (content.empty()) {
        return nullptr;
    }

    //Remove spaces at head and tail, and preserve at least one space.
    zaf::Trim(content);
    if (content.empty()) {
        content.append(1, L' ');
    }

    transaction.Commit();
    return element::MakeInlineCode(std::move(content));
}


bool InlineCodeParser::ParseHeadingBackquotes(
    ParseContext& context, 
    std::size_t& heading_backquote_count) const {

    heading_backquote_count = 0;

    while (context.CurrentChar() == L'`') {
        ++heading_backquote_count;
        context.Forward();
    }

    if (heading_backquote_count == 0) {
        return false;
    }

    return true;
}


void InlineCodeParser::TryToParseContinuousBackquotes(
    ParseContext& context,
    std::size_t& backquote_count) const {

    backquote_count = 0;
    while (context.CurrentChar() == L'`') {
        ++backquote_count;
        context.Forward();
    }
}

}