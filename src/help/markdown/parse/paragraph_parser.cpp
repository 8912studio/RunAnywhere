#include "help/markdown/parse/paragraph_parser.h"
#include <zaf/base/range.h>
#include <zaf/base/string/trim.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/span_element_parser.h"

namespace ra::help::markdown::parse {

ElementParser* ParagraphParser::Instance() {
    static ParagraphParser instance;
    return &instance;
}


std::shared_ptr<element::Element> ParagraphParser::Parse(ParseContext& context) {
    
    if (!context.IsAtLineStart()) {
        return nullptr;
    }

    std::vector<LineInfo> line_infos;
    while (!context.IsEnd()) {

        auto line_info = ParseOneLine(context);

        //Encounter an empty line.
        if (line_info.heading_text.empty() && 
            line_info.elements.empty() && 
            line_info.tailing_text.empty()) {
            break;
        }

        line_infos.push_back(std::move(line_info));
    }

    if (line_infos.empty()) {
        return nullptr;
    }

    //Merge line infos to elements.
    element::ElementList elements;
    for (auto index : zaf::Range(0, line_infos.size())) {

        LineInfo* prior_line{};
        if (index > 0) {
            prior_line = &line_infos[index - 1];
        }

        MergeLineInfo(prior_line, line_infos[index], elements);
    }

    auto& last_line = line_infos.back();
    if (!last_line.tailing_text.empty()) {
        elements.push_back(element::MakeText(std::move(last_line.tailing_text)));
    }

    return element::MakeParagraph(std::move(elements));
}


ParagraphParser::LineInfo ParagraphParser::ParseOneLine(ParseContext& context) {

    LineInfo result;
    std::wstring text_piece;

    while ((context.CurrentChar() != L'\n') && (context.CurrentChar() != L'\0')) {

        auto span_element = SpanElementParser::Instance()->Parse(context);
        if (span_element) {

            if (!text_piece.empty()) {

                //This is the heading text.
                if (result.elements.empty()) {

                    zaf::TrimStart(text_piece);
                    if (!text_piece.empty()) {
                        result.heading_text = std::move(text_piece);
                    }
                }
                //The is the middle text.
                else {

                    result.elements.push_back(element::MakeText(std::move(text_piece)));
                }
            }

            result.elements.push_back(std::move(span_element));
            continue;
        }

        text_piece.append(1, context.CurrentChar());
        context.Forward();
    }

    //Eat the last '\n' char
    context.Forward();

    if (!text_piece.empty()) {

        std::size_t tailing_space_count{};
        auto last_not_space_index = text_piece.find_last_not_of(L' ');
        if (last_not_space_index != std::wstring::npos) {
            tailing_space_count = text_piece.length() - (last_not_space_index + 1);
        }
        else {
            tailing_space_count = text_piece.length();
        }

        if (tailing_space_count >= 2) {
            result.has_tailing_spaces = true;
        }

        zaf::TrimEnd(text_piece);
        if (!text_piece.empty()) {

            if (result.elements.empty()) {

                zaf::TrimStart(text_piece);
                if (!text_piece.empty()) {
                    result.heading_text = std::move(text_piece);
                }
            }
            else {
                result.tailing_text = std::move(text_piece);
            }
        }
    }

    return result;
}


void ParagraphParser::MergeLineInfo(
    LineInfo* prior_line,
    LineInfo& current_line,
    element::ElementList& elements) {

    std::shared_ptr<element::Element> head_element;
    if (prior_line) {

        std::wstring merged_text = std::move(prior_line->tailing_text);
        if (prior_line->has_tailing_spaces) {
            merged_text.append(1, L'\n');
        }
        else {
            merged_text.append(1, L' ');
        }
        merged_text.append(current_line.heading_text);

        head_element = element::MakeText(std::move(merged_text));
    }
    else {

        //Move heading text to tailing text if there is only text in the first line.
        if (current_line.elements.empty()) {
            current_line.tailing_text = std::move(current_line.heading_text);
        }
        else if (!current_line.heading_text.empty()) {
            head_element = element::MakeText(std::move(current_line.heading_text));
        }
    }

    if (head_element) {
        elements.push_back(std::move(head_element));
    }

    for (auto& each_element : current_line.elements) {
        elements.push_back(std::move(each_element));
    }
}

}