#include "utility/markdown/parse/emphasis_parser.h"
#include "utility/markdown/element/factory.h"

namespace ra::help::markdown::parse {

std::shared_ptr<element::Element> EmphasisParser::Parse(ParseContext& context) {

    auto transaction = context.BeginTransaction();

    //Parse heading identity chars.
    if (!ParseIdentityChars(context)) {
        return nullptr;
    }

    //Not allow space between heading identity chars and content.
    if (context.CurrentChar() == L' ') {
        return nullptr;
    }

    element::ElementList children;
    std::wstring text;

    while (!context.IsAtLineEnd()) {

        auto child_element = ParseNextChildElement(context);
        if (child_element) {

            if (!text.empty()) {
                children.push_back(element::MakeText(std::move(text)));
            }

            children.push_back(std::move(child_element));
            continue;
        }
        else if (context.CurrentChar() != options_.identity_char) {
            text.append(1, context.CurrentChar());
            context.Forward();
        }
        else {
            break;
        }
    }

    //Parse tailing identity chars.
    if (!ParseIdentityChars(context)) {
        return nullptr;
    }

    if (!text.empty()) {

        //Not allow space between content and tailing identity chars.
        if (text.back() == L' ') {
            return nullptr;
        }

        children.push_back(element::MakeText(std::move(text)));
    }

    if (children.empty()) {
        return nullptr;
    }

    transaction.Commit();
    return std::make_shared<element::Element>(options_.element_type, std::move(children));
}


bool EmphasisParser::ParseIdentityChars(ParseContext& context) const {

    std::size_t identity_char_count{};
    while (context.CurrentChar() == options_.identity_char) {

        ++identity_char_count;
        context.Forward();

        if (identity_char_count == options_.identity_char_count) {
            return true;
        }
    }
    return false;
}


std::shared_ptr<element::Element> EmphasisParser::ParseNextChildElement(
    ParseContext& context) const {

    for (auto each_parser : options_.children_parsers) {

        auto child_element = each_parser->Parse(context);
        if (child_element) {
            return child_element;
        }
    }

    return nullptr;
}

}
