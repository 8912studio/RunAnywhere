#include "help/markdown/parse/header_parser.h"
#include <zaf/base/error/check.h>
#include <zaf/base/string/trim.h>
#include "help/markdown/element/factory.h"

namespace ra::help::markdown::parse {

HeaderParser::Status HeaderParser::ParseOneLine(ParseContext& context) {

    ZAF_EXPECT(!state_.has_value());

    auto transaction = context.BeginTransaction();
    context.SkipSpaces();

    std::size_t hash_count{};
    while (context.CurrentChar() == L'#') {
        ++hash_count;
        context.Forward();
    }

    if (hash_count == 0 || hash_count > 6) {
        return Status::Failed;
    }

    if (context.CurrentChar() != L' ') {
        return Status::Failed;
    }

    context.Forward();

    if (context.IsAtLineEnd()) {
        return Status::Failed;
    }

    std::wstring content;
    do {
        content.append(1, context.CurrentChar());
        context.Forward();
    } while (!context.IsAtLineEnd());

    //Trim spaces
    zaf::Trim(content);

    //Remove tailing hashes
    auto last_not_hash_index = content.find_last_not_of(L'#');
    if (last_not_hash_index == std::wstring::npos) {
        return Status::Failed;
    }

    content.erase(last_not_hash_index + 1);
    //Trim spaces again.
    zaf::Trim(content);

    if (content.empty()) {
        return Status::Failed;
    }

    //Eat the tailing line break.
    context.Forward();
    transaction.Commit();

    state_.emplace();
    state_->hash_count = hash_count;
    state_->content = std::move(content);
    return Status::Finished;
}


std::shared_ptr<element::Element> HeaderParser::FinishCurrentElement() {

    ZAF_EXPECT(state_.has_value());

    auto depth = std::size_t(element::HeaderDepth::_1) + state_->hash_count - 1;
    auto result = element::MakeHeader(element::HeaderDepth(depth), std::move(state_->content));
    state_.reset();
    return result;
}

}