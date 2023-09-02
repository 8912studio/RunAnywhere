#include <gtest/gtest.h>
#include "help/markdown/markdown_parser.h"

using namespace ra::help::markdown;

TEST(MarkdownParserTest, Parse) {

    ParseContext context(L"# head\n tst");

    StyledStringBuilder builder;

    MarkdownParser parser;
    auto status = parser.Parse(context, builder);

    auto styled_string = builder.Build();

    auto s = styled_string.String();
}