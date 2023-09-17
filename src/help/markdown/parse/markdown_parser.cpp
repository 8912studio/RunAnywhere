#include "help/markdown/parse/markdown_parser.h"
#include <zaf/base/error/check.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/body_parser.h"

namespace ra::help::markdown::parse {

MarkdownParser* MarkdownParser::Instance() {
    static MarkdownParser instance;
    return &instance;
}


std::shared_ptr<element::Element> MarkdownParser::Parse(std::wstring_view input) {

    ParseContext context(input);

    BodyParser body_parser;
    while (!context.IsEnd()) {

        ZAF_EXPECT(context.IsAtLineStart());
        body_parser.ParseOneLine(context);
    }

    auto elements = body_parser.Finish();
    return element::MakeRoot(std::move(elements));
}

}