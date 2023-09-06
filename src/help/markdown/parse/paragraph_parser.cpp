#include "help/markdown/parse/paragraph_parser.h"
#include "help/markdown/parse/span_element_parser.h"

namespace ra::help::markdown::parse {
namespace {

void AppendLineBreak(std::wstring& content) {

    if (content.empty()) {
        content.append(1, L'\n');
        return;
    }

    auto last_not_space_index = content.find_last_not_of(L' ');
    //Whole line is spaces.
    if (last_not_space_index == std::wstring::npos) {


    }
    else {

    }
}

}

ElementParser* ParagraphParser::Instance() {
    static ParagraphParser instance;
    return &instance;
}


std::shared_ptr<element::Element> ParagraphParser::Parse(ParseContext& context) {
    
    if (!context.IsAtLineStart()) {
        return nullptr;
    }

    element::ElementList elements;
    std::optional<LineInfo> last_line_info;

    while (true) {

        auto line_info = ParseOneLine(context);

    }

    return std::make_shared<element::Element>(
        element::ElementType::Paragraph,
        std::move(elements));
}


ParagraphParser::LineInfo ParagraphParser::ParseOneLine(ParseContext& context) {

    element::ElementList elements;
    std::wstring plain_text;

    while ((context.GetCurrentChar() != L'\n') && (context.GetCurrentChar() != L'\0')) {

        auto span_element = SpanElementParser::Instance()->Parse(context);
        if (span_element) {

            if (!plain_text.empty()) {
                elements.push_back(std::make_shared<element::Element>(std::move(plain_text)));
            }

            elements.push_back(std::move(span_element));
            continue;
        }

        plain_text.append(1, context.GetCurrentChar());
        context.Forward(1);
    }

    LineInfo line_info;

    if (!plain_text.empty()) {

        std::size_t space_count{};
        auto last_not_space_index = plain_text.find_last_not_of(L' ');
        if (last_not_space_index != std::wstring::npos) {

            space_count = plain_text.length() - (last_not_space_index + 1);
            plain_text.erase(last_not_space_index + 1);
        }
        else {

            space_count = plain_text.length();
            plain_text.clear();
        }

        if (space_count >= 2) {
            line_info.has_concatenate_mark = true;
        }

        if (!plain_text.empty()) {
            elements.push_back(std::make_shared<element::Element>(std::move(plain_text)));
        }
    }

    line_info.elements = std::move(elements);
    return line_info;
}

}