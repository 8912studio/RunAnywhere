#include <gtest/gtest.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/render/styled_text_builder.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::render;

TEST(StyledTextBuilderTest, Build) {

    TextStyle basic_style;
    basic_style.font = zaf::Font::Default();

    {
        StyledTextBuilder builder;
        auto styled_text = builder.Build(*MakeParagraph(L"Paragraph"), basic_style);
        ASSERT_EQ(styled_text.Text(), L"Paragraph");
        ASSERT_EQ(styled_text.Styles().size(), 1);
        ASSERT_EQ(styled_text.Styles().front().range, zaf::Range(0, 9));
    }

    {
        auto element = MakeParagraph({
            MakeText(L"text1 "),
            MakeBold(L"bold"),
            MakeText(L" text2"),
        });
        StyledTextBuilder builder;
        auto styled_text = builder.Build(*element, basic_style);
        ASSERT_EQ(styled_text.Text(), L"text1 bold text2");
        ASSERT_EQ(styled_text.Styles().size(), 3);
        ASSERT_EQ(styled_text.Styles()[0].range, zaf::Range(0, 6));
        ASSERT_EQ(styled_text.Styles()[1].range, zaf::Range(6, 4));
        ASSERT_EQ(styled_text.Styles()[2].range, zaf::Range(10, 6));
    }
}