#include "help/markdown/parse/code_block_parser.h"
#include "help/markdown/element/factory.h"

namespace ra::help::markdown::parse {

ElementParser* CodeBlockParser::Instance() {
    static CodeBlockParser instance;
    return &instance;
}


std::shared_ptr<element::Element> CodeBlockParser::Parse(ParseContext& context) {

    if (!context.IsAtLineStart()) {
        return nullptr;
    }

    auto transaction = context.BeginTransaction();

    std::size_t backquote_count{};
    if (!ParseHeadingLine(context, backquote_count)) {
        return nullptr;
    }

    std::wstring content;

    while (!context.IsEnd()) {

        if (context.IsAtLineStart()) {
            if (ParseTailingLine(context, backquote_count)) {
                //Remove the last line break.
                if (!content.empty()) {
                    content.pop_back();
                }
                break;
            }
        }

        content.append(1, context.CurrentChar());
        context.Forward();
    }

    transaction.Commit();
    return element::MakeCodeBlock(std::move(content));
}


bool CodeBlockParser::ParseHeadingLine(ParseContext& context, std::size_t& backquote_count) const {

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
    return true;
}


bool CodeBlockParser::ParseTailingLine(
    ParseContext& context, 
    std::size_t heading_backquote_count) const {

    auto transaction = context.BeginTransaction();

    context.SkipSpaces();

    std::size_t tailing_backquote_count{};
    while (context.CurrentChar() == L'`') {
        ++tailing_backquote_count;
        context.Forward();
    }

    if (tailing_backquote_count < heading_backquote_count) {
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

}