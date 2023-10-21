#include "utility/markdown/parse/header_parser.h"
#include <zaf/base/error/check.h>
#include <zaf/base/string/trim.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/span_element_parser.h"

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

    auto children = ParseChildren(context);
    if (children.empty()) {
        return Status::Failed;
    }

    //Eat the tailing line break.
    context.Forward();
    transaction.Commit();

    state_.emplace();
    state_->hash_count = hash_count;
    state_->children = std::move(children);
    return Status::Finished;
}


element::ElementList HeaderParser::ParseChildren(ParseContext& context) {

    element::ElementList result;
    context.SkipSpaces();

    std::wstring text;
    while (!context.IsAtLineEnd()) {

        auto span = SpanElementParser::Instance()->Parse(context);
        if (span) {

            if (!text.empty()) {
                result.push_back(element::MakeText(std::move(text)));
            }

            result.push_back(std::move(span));
        }
        else {

            text.append(1, context.CurrentChar());
            context.Forward();
        }
    }

    zaf::TrimEnd(text);

    //Remove tailing hashes
    auto last_not_hash_index = text.find_last_not_of(L'#');
    if (last_not_hash_index == std::wstring::npos) {
        text.clear();
    }
    else {
        text.erase(last_not_hash_index + 1);
        zaf::TrimEnd(text);
    }

    if (!text.empty()) {
        result.push_back(element::MakeText(std::move(text)));
    }

    return result;
}


HeaderParser::Result HeaderParser::FinishCurrentElement() {

    ZAF_EXPECT(state_.has_value());

    auto depth = std::size_t(element::HeaderDepth::_1) + state_->hash_count - 1;
    auto result = element::MakeHeader(element::HeaderDepth(depth), std::move(state_->children));
    state_.reset();
    return result;
}

}