#include "help/markdown/parse/header_parser.h"
#include <zaf/base/string/trim.h>
#include "help/markdown/element/header_element.h"

namespace ra::help::markdown::parse {

ElementParser* HeaderParser::Instance() {
    static HeaderParser instance;
    return &instance;
}


std::shared_ptr<element::Element> HeaderParser::Parse(ParseContext& context) {

    if (!context.IsAtLineStart()) {
        return nullptr;
    }

    auto reader = context.BeginRead();

    reader.SkipSpaces();

    std::size_t hash_count{};
    while (reader.GetChar() == L'#') {
        ++hash_count;
        reader.Forward();
    }

    if (hash_count == 0 || hash_count > 3) {
        reader.Discard();
        return nullptr;
    }

    if (reader.GetChar() != L' ') {
        reader.Discard();
        return nullptr;
    }

    reader.Forward();

    if (reader.IsAtLineEnd()) {
        reader.Discard();
        return nullptr;
    }

    std::wstring content;
    do {
        content.append(1, reader.GetChar());
        reader.Forward();
    } 
    while (!reader.IsAtLineEnd());

    //Trim spaces
    zaf::Trim(content);

    //Remove tailing hashes
    auto last_not_hash_index = content.find_last_not_of(L'#');
    if (last_not_hash_index == std::wstring::npos) {
        reader.Discard();
        return nullptr;
    }

    content.erase(last_not_hash_index + 1);
    //Trim spaces again.
    zaf::Trim(content);

    if (content.empty()) {
        reader.Discard();
        return nullptr;
    }

    auto depth = element::HeaderDepth(std::size_t(element::HeaderDepth::_1) + hash_count - 1);
    return std::make_shared<element::HeaderElement>(depth, element::ElementList{
        std::make_shared<element::Element>(std::move(content))
    });
}

}