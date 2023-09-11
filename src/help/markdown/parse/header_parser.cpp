#include "help/markdown/parse/header_parser.h"
#include <zaf/base/string/trim.h>
#include "help/markdown/element/factory.h"

namespace ra::help::markdown::parse {

ElementParser* HeaderParser::Instance() {
    static HeaderParser instance;
    return &instance;
}


std::shared_ptr<element::Element> HeaderParser::Parse(ParseContext& context) {

    if (!context.IsAtLineStart()) {
        return nullptr;
    }

    auto transaction = context.BeginTransaction();

    context.SkipSpaces();

    std::size_t hash_count{};
    while (context.CurrentChar() == L'#') {
        ++hash_count;
        context.Forward();
    }

    if (hash_count == 0 || hash_count > 3) {
        return nullptr;
    }

    if (context.CurrentChar() != L' ') {
        return nullptr;
    }

    context.Forward();

    if (context.IsAtLineEnd()) {
        return nullptr;
    }

    std::wstring content;
    do {
        content.append(1, context.CurrentChar());
        context.Forward();
    } 
    while (!context.IsAtLineEnd());

    //Trim spaces
    zaf::Trim(content);

    //Remove tailing hashes
    auto last_not_hash_index = content.find_last_not_of(L'#');
    if (last_not_hash_index == std::wstring::npos) {
        return nullptr;
    }

    content.erase(last_not_hash_index + 1);
    //Trim spaces again.
    zaf::Trim(content);

    if (content.empty()) {
        return nullptr;
    }

    transaction.Commit();

    auto depth = element::HeaderDepth(std::size_t(element::HeaderDepth::_1) + hash_count - 1);
    return element::MakeHeader(depth, {
        element::MakeText(std::move(content)),
    });
}

}