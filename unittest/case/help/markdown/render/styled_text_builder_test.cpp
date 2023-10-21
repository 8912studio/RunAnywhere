#include <gtest/gtest.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/render/styled_text_builder.h"

using namespace ra::utility::markdown::element;
using namespace ra::utility::markdown::render;

TEST(StyledTextBuilderTest, Build) {

    StyleConfig style_config;
    style_config.basic_config.font = zaf::Font::Default();

    {
        StyledTextBuilder builder;
        auto styled_text = builder.Build(*MakeParagraph(L"Paragraph"), style_config);
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
        auto styled_text = builder.Build(*element, style_config);
        ASSERT_EQ(styled_text.Text(), L"text1 bold text2");
        ASSERT_EQ(styled_text.Styles().size(), 3);
        ASSERT_EQ(styled_text.Styles()[0].range, zaf::Range(0, 6));
        ASSERT_EQ(styled_text.Styles()[1].range, zaf::Range(6, 4));
        ASSERT_EQ(styled_text.Styles()[2].range, zaf::Range(10, 6));
    }
}