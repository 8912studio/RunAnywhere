#include "utility/markdown/parse/emphasis_parser.h"
#include "utility/markdown/element/factory.h"

namespace ra::utility::markdown::parse {

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
    bool has_parsed_tailing_identity_chars{};

    while (!context.IsAtLineEnd()) {

        auto child_element = ParseNextChildElement(context);
        if (child_element) {

            if (!text.empty()) {
                children.push_back(element::MakeText(std::move(text)));
            }

            children.push_back(std::move(child_element));
        }
        else if (context.CurrentChar() != options_.identity_char) {
            text.append(1, context.CurrentChar());
            context.Forward();
        }
        //Encounter an identity char.
        else {

            //If there is no content between the heading identity chars and the current identity 
            //char, break the loop.
            if (text.empty() && children.empty()) {
                break;
            }

            //Try to parse the tailing identity chars.
            {
                auto inner_transaction = context.BeginTransaction();
                if (ParseIdentityChars(context)) {
                    inner_transaction.Commit();
                    has_parsed_tailing_identity_chars = true;
                    break;
                }
            }

            //Parsing identity chars fails, the current identity char is a part of the text.
            text.append(1, options_.identity_char);
            context.Forward();
        }
    }

    if (!has_parsed_tailing_identity_chars) {
        if (!ParseIdentityChars(context)) {
            return nullptr;
        }
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
