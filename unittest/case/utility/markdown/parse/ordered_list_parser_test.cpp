#include <gtest/gtest.h>
#include <zaf/base/range.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/ordered_list_parser.h"

using namespace ra::utility::markdown::element;
using namespace ra::utility::markdown::parse;

TEST(OrderedListParserTest, Parse) {

    auto test = [](
        std::wstring_view input,
        std::size_t line_count, 
        const std::shared_ptr<Element>& element) {
    
        ParseContext context{ input };
        OrderedListParser parser;

        for (auto line_index : zaf::Range(0, line_count)) {
            auto status = parser.ParseOneLine(context);
            if (status != BlockParser::Status::Continue) {
                return false;
            }
        }

        auto result = parser.FinishCurrentElement();
        return result.element->IsEqualTo(*element);
    };

    ASSERT_TRUE(test(
LR"(1. item1
2. item2
3. item3)",
        3,
        MakeOrderedList(ListItemStyle::Lines, 1, {
            MakeListItem({ MakeParagraph(L"item1")}),
            MakeListItem({ MakeParagraph(L"item2")}),
            MakeListItem({ MakeParagraph(L"item3")}),
        })));

    
    ASSERT_TRUE(test(
LR"(1. item1
abc
2. item2
dd
3. item3)",
        5,
        MakeOrderedList(ListItemStyle::Lines, 1, {
            MakeListItem({ MakeParagraph(L"item1 abc")}),
            MakeListItem({ MakeParagraph(L"item2 dd")}),
            MakeListItem({ MakeParagraph(L"item3")}),
        })));

    ASSERT_TRUE(test(
LR"(1. item1

   abc

2. item2

    def

3. item3)",
        9,
        MakeOrderedList(ListItemStyle::Blocks, 1, {
            MakeListItem({ 
                MakeParagraph(L"item1"),
                MakeParagraph(L"abc")
            }),
            MakeListItem({
                MakeParagraph(L"item2"),
                MakeParagraph(L"def")
            }),
            MakeListItem({ MakeParagraph(L"item3") }),
        })));
}