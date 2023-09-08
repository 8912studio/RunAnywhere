#include "help/markdown/parse/emphasis_parser.h"
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/inline_code_parser.h"

namespace ra::help::markdown::parse {

ElementParser* EmphasisParser::Instance() {
    static EmphasisParser instance;
    return &instance;
}


std::shared_ptr<element::Element> EmphasisParser::Parse(ParseContext& context) {

    std::size_t star_count{};

    auto reader = context.BeginRead();

    //Emphasis starts exactly with 2 star chars.
    while (reader.GetChar() == L'*') {

        ++star_count;
        reader.Forward();

        if (star_count > 2) {
            reader.Discard();
            return nullptr;
        }
    }

    //Not allow space between heading star and content.
    if (reader.GetChar() == L' ') {
        reader.Discard();
        return nullptr;
    }

    element::ElementList children;
    std::wstring text;

    while (reader.GetChar() != L'*' && !reader.IsAtLineEnd()) {

        auto inline_code = InlineCodeParser::Instance()->Parse(context);
        if (inline_code) {

            if (!text.empty()) {
                children.push_back(element::MakeText(std::move(text)));
            }

            children.push_back(std::move(inline_code));
            continue;
        }
        else {
            text.append(1, reader.GetChar());
            reader.Forward();
        }
    }

    //Emphasis ends exactly with 2 star chars.
    for (std::size_t tailing_star_count = 0; tailing_star_count < 2; ++tailing_star_count) {
    
        if (reader.GetChar() != L'*') {
            reader.Discard();
            return nullptr;
        }

        reader.Forward();
    }

    if (!text.empty()) {

        //Not allow space between content and tailing star.
        if (text.back() == L' ') {
            reader.Discard();
            return nullptr;
        }

        children.push_back(element::MakeText(std::move(text)));
    }

    return element::MakeEmphasis(std::move(children));
}

}