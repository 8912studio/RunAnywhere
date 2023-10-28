#include <gtest/gtest.h>
#include <zaf/base/range.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/ordered_list_item_parser.h"

using namespace ra::utility::markdown::element;
using namespace ra::utility::markdown::parse;

TEST(OrderedListItemParserTest, Parse) {

    auto test = [](
        std::wstring_view input,
        std::size_t line_count,
        std::size_t indent_char_count,
        std::size_t number,
        const std::shared_ptr<Element>& element) {
    
        ParseContext context{ input };
        OrderedListItemParser parser;

        for (auto line_index : zaf::Range(0, line_count)) {
            auto status = parser.ParseOneLine(context);
            if (status != BlockParser::Status::Continue) {
                return false;
            }
        }

        if ((parser.IndentCharCount() != indent_char_count) || (parser.Number() != number)) {
            return false;
        }

        auto result = parser.FinishCurrentElement();
        return result.element->IsEqualTo(*element);
    };

    ASSERT_TRUE(test(L"1. item", 1, 3, 1, MakeListItem({
        MakeParagraph(L"item")
    })));

    ASSERT_TRUE(test(L"123.   item", 1, 7, 123, MakeListItem({
        MakeParagraph(L"item")
    })));

    ASSERT_TRUE(test(L"2. item\nabc", 2, 3, 2, MakeListItem({
        MakeParagraph(L"item abc")
    })));

    ASSERT_TRUE(test(L"3. item\n\n   abc", 3, 3, 3, MakeListItem({
        MakeParagraph(L"item"),
        MakeParagraph(L"abc")
    })));

    ASSERT_TRUE(test(L"5. item\n\n     abc", 3, 3, 5, MakeListItem({
        MakeParagraph(L"item"),
        MakeParagraph(L"abc")
    })));

    ASSERT_TRUE(test(L" 4.   item\n\n      abc", 3, 6, 4, MakeListItem({
        MakeParagraph(L"item"),
        MakeParagraph(L"abc")
    })));
}