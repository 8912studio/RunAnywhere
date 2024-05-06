#include "utility/markdown/parse/markdown_parser.h"
#include <zaf/base/error/contract_error.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/body_parser.h"

namespace ra::utility::markdown::parse {

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

    auto body_result = body_parser.Finish();
    return element::MakeRoot(std::move(body_result.elements));
}

}