#include "help/markdown/parse/bold_parser.h"
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/inline_code_parser.h"

namespace ra::help::markdown::parse {

ElementParser* BoldParser::Instance() {
    static BoldParser instance;
    return &instance;
}


std::shared_ptr<element::Element> BoldParser::Parse(ParseContext& context) {

    auto transaction = context.BeginTransaction();

    //Bold starts exactly with 2 star chars.
    std::size_t star_count{};
    while (context.CurrentChar() == L'*') {

        ++star_count;
        context.Forward();

        if (star_count > 2) {
            return nullptr;
        }
    }

    if (star_count != 2) {
        return nullptr;
    }

    //Not allow space between heading star and content.
    if (context.CurrentChar() == L' ') {
        return nullptr;
    }

    element::ElementList children;
    std::wstring text;

    while (context.CurrentChar() != L'*' && !context.IsAtLineEnd()) {

        auto inline_code = InlineCodeParser::Instance()->Parse(context);
        if (inline_code) {

            if (!text.empty()) {
                children.push_back(element::MakeText(std::move(text)));
            }

            children.push_back(std::move(inline_code));
            continue;
        }
        else {
            text.append(1, context.CurrentChar());
            context.Forward();
        }
    }

    //Bold ends exactly with 2 star chars.
    for (std::size_t tailing_star_count = 0; tailing_star_count < 2; ++tailing_star_count) {
    
        if (context.CurrentChar() != L'*') {
            return nullptr;
        }

        context.Forward();
    }

    if (!text.empty()) {

        //Not allow space between content and tailing star.
        if (text.back() == L' ') {
            return nullptr;
        }

        children.push_back(element::MakeText(std::move(text)));
    }

    transaction.Commit();
    return element::MakeBold(std::move(children));
}

}