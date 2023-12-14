#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <zaf/base/string/encoding_conversion.h>
#include "utility/markdown/element/factory.h"
#include "utility/markdown/parse/markdown_parser.h"

using namespace ra::utility::markdown::element;
using namespace ra::utility::markdown::parse;

namespace {

bool TestParser(std::wstring_view input, ElementList expected) {
    auto element = MarkdownParser::Instance()->Parse(input);
    if (!element) {
        return false;
    }
    return element->IsEqualTo(*MakeRoot(std::move(expected)));
}


std::wstring ReadTestFile(std::wstring_view filename) {

    std::filesystem::path path = __FILEW__;
    path = path.parent_path() / filename;

    std::ifstream file_stream(path, std::ios::in | std::ios::binary);
    if (!file_stream) {
        return {};
    }
    file_stream.seekg(0, std::ios::end);
    auto file_length = file_stream.tellg();

    std::string buffer;
    buffer.resize(file_length);

    file_stream.seekg(0, std::ifstream::beg);
    file_stream.read(&buffer[0], file_length);

    return zaf::FromUTF8String(buffer);
}

}

TEST(MarkdownParserTest, ParseFiles) {

    auto test = [](std::wstring_view filename, ElementList expected) {
        auto input = ReadTestFile(filename);
        return TestParser(input, std::move(expected));
    };

    ASSERT_TRUE(test(L"markdown_parser_test_1.md", {
        MakeHeader(HeaderDepth::_1, L"Hex command help"),
        MakeParagraph(L"Display content of file in hex byte format."),
        MakeParagraph({
            MakeBold(L"Usage")
        }),
        MakeCodeBlock(L"hex [text] [position] [length] /f /u8 /u16"),
        MakeParagraph({
            MakeInlineCode(L"position"),
            MakeText(L" starts with "),
            MakeInlineCode(L"`"),
            MakeText(L". "),
            MakeInlineCode(L"length"),
            MakeText(L" starts with "),
            MakeInlineCode(L"~"),
            MakeText(L", followed by a number."),
        }),
        MakeParagraph({ 
            MakeBold(L"Switches")
        }),
        MakeUnorderedList(ListItemStyle::Blocks, {
            MakeListItem({
                MakeParagraph({ MakeInlineCode(L"/f") }),
                MakeParagraph({
                    MakeText(L"Treats "),
                    MakeInlineCode(L"text"),
                    MakeText(L" as file path."),
                }),
            }),
            MakeListItem({
                MakeParagraph({ MakeInlineCode(L"/u8") }),
                MakeParagraph({
                    MakeText(L"Treats active path as string and display it in UTF-8 encoding."),
                }),
            }),
            MakeListItem({
                MakeParagraph({ MakeInlineCode(L"/u16") }),
                MakeParagraph({
                    MakeText(L"Treats active path as string and display it in UTF-16 encoding."),
                }),
            }),
        }),
        MakeParagraph({
            MakeText(L"Example:"),
        }),
        MakeCodeBlock(L"hex `32 ~16\nhex \"string to display\" /u8"),
    }));

    ASSERT_TRUE(test(L"markdown_parser_test_2.md", {
        MakeParagraph(L"Here's how you can do it:"),
        MakeOrderedList(ListItemStyle::Lines, 1, {
            MakeListItem({
                MakeParagraph(L"Include the header file at the top of your source file:"),
            }),
        }),
        MakeCodeBlock(L"#include <QDebug>"),
        MakeOrderedList(ListItemStyle::Lines, 2, {
            MakeListItem({
                MakeParagraph(L"Use the qDebug() function to print the debug information:"),
            }),
        }),
        MakeCodeBlock(LR"(qDebug() << "Debug information";)"),
        MakeOrderedList(ListItemStyle::Lines, 3, {
            MakeListItem({
                MakeParagraph(L"You can also include variables or values in the debug output:")
            }),
        }),
        MakeCodeBlock(L"int value = 42;\nqDebug() << \"The value is:\" << value;"),
        MakeOrderedList(ListItemStyle::Blocks, 4, {
            MakeListItem({
                MakeParagraph(L"To build and run the application with debug output."),
            }),
            MakeListItem({
                MakeParagraph(L"When you run the application,"),
            })
        }),
        MakeParagraph(L"Note: qDebug() statements are only displayed in the console"),
        MakeParagraph(L"Here's an example of using qInfo() for release mode:"),
        MakeCodeBlock(LR"(qInfo() << "Information message";)"),
        MakeParagraph(L"Additionally, you can also redirect the debug output to a file"),
    }));
}


TEST(MarkdownParserTest, EmptyLines) {

    ASSERT_TRUE(TestParser(L"", {}));
    ASSERT_TRUE(TestParser(L"\n\n\nline1\n\nline2\nline3\n", {
        MakeParagraph(L"line1"),
        MakeParagraph(L"line2 line3"),
    }));
    ASSERT_TRUE(TestParser(L"\n  \n   \nline1\n\nline2\n  \n", {
        MakeParagraph(L"line1"),
        MakeParagraph(L"line2"),
    }));
}


TEST(MarkdownParserTest, ParseParagraph) {

    auto test = [](std::wstring_view input, ElementList expected) {
        return TestParser(input, { MakeParagraph(std::move(expected)) });
    };

    ASSERT_TRUE(test(L"single line paragraph", { MakeText(L"single line paragraph") }));
    ASSERT_TRUE(test(L"line1\nline2", { MakeText(L"line1 line2") }));
    ASSERT_TRUE(test(L"line1\nline2\n", { MakeText(L"line1 line2") }));
    ASSERT_TRUE(test(L"line1\nline2\nline3", { MakeText(L"line1 line2 line3") }));
    ASSERT_TRUE(test(L"line1 \nline2", { MakeText(L"line1 line2") }));
    ASSERT_TRUE(test(L"line1 \n line2", { MakeText(L"line1 line2") }));
    ASSERT_TRUE(test(L"line1  \nline2", { MakeText(L"line1\nline2") }));
    ASSERT_TRUE(test(L"line1   \n line2", { MakeText(L"line1\nline2") }));

    ASSERT_TRUE(test(L"``", { MakeText(L"``") }));
    ASSERT_TRUE(test(L"``text", { MakeText(L"``text") }));
    ASSERT_TRUE(test(L"``text`", { MakeText(L"``text`") }));
    ASSERT_TRUE(test(L"text`", { MakeText(L"text`") }));
    ASSERT_TRUE(test(L"`text``", { MakeText(L"`text``") }));
    ASSERT_TRUE(test(L"`code`", { MakeInlineCode(L"code") }));
    ASSERT_TRUE(test(L" `code`  ", { MakeInlineCode(L"code") }));

    ASSERT_TRUE(test(L"``text`code`  ", {
        MakeText(L"``text"),
        MakeInlineCode(L"code"),
    }));

    ASSERT_TRUE(test(L"`code`\ntext", {
        MakeInlineCode(L"code"),
        MakeText(L" text"),
    }));

    ASSERT_TRUE(test(L"`code` \ntext", {
        MakeInlineCode(L"code"),
        MakeText(L" text"),
    }));

    ASSERT_TRUE(test(L"`code`  \ntext", {
        MakeInlineCode(L"code"),
        MakeText(L"\ntext"),
    }));

    ASSERT_TRUE(test(L"text\n`code`", {
        MakeText(L"text "),
        MakeInlineCode(L"code"),
    }));

    ASSERT_TRUE(test(L"This is an `inline code`.", {
        MakeText(L"This is an "),
        MakeInlineCode(L"inline code"),
        MakeText(L"."),
    }));

    ASSERT_TRUE(test(L"**`code**`", {
        MakeText(L"**"),
        MakeInlineCode(L"code**"),
    }));

    ASSERT_TRUE(test(L"***abc***", {
        MakeItalic({
            MakeBold({ MakeText(L"abc") })
        })
    }));

    ASSERT_TRUE(test(L"***abc**", {
        MakeText(L"*"),
        MakeBold({ MakeText(L"abc") }),
    }));

    ASSERT_TRUE(test(L"**abc***", {
        MakeBold({ MakeText(L"abc") }),
        MakeText(L"*"),
    }));
}



TEST(MarkdownParserTest, ParseCodeBlock) {

    auto test = [](std::wstring_view input, std::wstring_view expected) {
        return TestParser(input, { MakeCodeBlock(std::wstring(expected)) });
    };

    ASSERT_TRUE(test(L"```\n```", L""));
    ASSERT_TRUE(test(L"```\n\n```", L""));
    ASSERT_TRUE(test(L"```\n\n\n```", L"\n"));
    ASSERT_TRUE(test(L"```\nabc\n```", L"abc"));
    ASSERT_TRUE(test(L"```\nabc \n    define\n```", L"abc \n    define"));
    ASSERT_TRUE(test(L"````\nabcd\n````", L"abcd"));
    ASSERT_TRUE(test(L"````\nabcd\n```````", L"abcd"));
    ASSERT_TRUE(test(L"```\nline1\nline2\nline3", L"line1\nline2\nline3"));
    ASSERT_TRUE(test(L"```\n``\n```", L"``"));
}


TEST(MarkdownParserTest, ParseUnorderedList) {

    auto test = [](std::wstring_view input, ListItemStyle item_style, ElementList expected) {
        return TestParser(input, { MakeUnorderedList(item_style, std::move(expected)) });
    };

    ASSERT_TRUE(test(L"* item1\n* item2\n* item3\n", ListItemStyle::Lines, {
        MakeListItem({ MakeParagraph(L"item1") }),
        MakeListItem({ MakeParagraph(L"item2") }),
        MakeListItem({ MakeParagraph(L"item3") }),
    }));

    //Spaces at the head of lines.
    ASSERT_TRUE(test(L" * item1\n  * item2\n  * item3\n", ListItemStyle::Lines, {
        MakeListItem({ MakeParagraph(L"item1") }),
        MakeListItem({ MakeParagraph(L"item2") }),
        MakeListItem({ MakeParagraph(L"item3") }),
    }));

    ASSERT_TRUE(test(L"* item1\n\n* item2\n\n\n* item3\n\n", ListItemStyle::Blocks, {
        MakeListItem({ MakeParagraph(L"item1") }),
        MakeListItem({ MakeParagraph(L"item2") }),
        MakeListItem({ MakeParagraph(L"item3") }),
    }));

    ASSERT_TRUE(test(L"* item1\ntext1\n* item2\ntext1\ntext2", ListItemStyle::Lines, {
        MakeListItem({ MakeParagraph(L"item1 text1") }),
        MakeListItem({ MakeParagraph(L"item2 text1 text2") }),
    }));

    ASSERT_TRUE(test(
        L"* item1\n\n   text1\n\n* item2\n\n    text2\n* item3",
        ListItemStyle::Blocks, {

        MakeListItem({
            MakeParagraph(L"item1"), 
            MakeParagraph(L"text1"),
        }),
        MakeListItem({
            MakeParagraph(L"item2"),
            MakeParagraph(L"text2"),
        }),
        MakeListItem({
            MakeParagraph(L"item3"),
        }),
    }));

    ASSERT_TRUE(test(
LR"(- item1
    + item1-1
   + item1-2
- item2)",
        ListItemStyle::Lines, 
        {
            MakeListItem({
                MakeParagraph(L"item1"),
                MakeUnorderedList(ListItemStyle::Lines, {
                    MakeListItem({ MakeParagraph(L"item1-1") }),
                    MakeListItem({ MakeParagraph(L"item1-2") }),
                }),
            }),
            MakeListItem({
                MakeParagraph(L"item2"),
            }),
        }));

    ASSERT_TRUE(test(L"* item1\n\n    - subitem\n\n    paragraph", ListItemStyle::Blocks, {
        MakeListItem({
            MakeParagraph(L"item1"),
            MakeUnorderedList(ListItemStyle::Lines, {
                MakeListItem({ MakeParagraph(L"subitem") }),
            }),
            MakeParagraph(L"paragraph")
        }),
    }));

    //Paragraph in sub list.
    ASSERT_TRUE(test(
LR"(* item1
   - subitem

     paragraph)", ListItemStyle::Blocks, {
        MakeListItem({
            MakeParagraph(L"item1"),
            MakeUnorderedList(ListItemStyle::Blocks, {
                MakeListItem({ 
                    MakeParagraph(L"subitem"),
                    MakeParagraph(L"paragraph") 
                }),
            }),
        }),
    }));

    //Paragraph not in sub list.
    ASSERT_TRUE(test(
LR"(* item1
   - subitem

    paragraph)", ListItemStyle::Blocks, {
        MakeListItem({
            MakeParagraph(L"item1"),
            MakeUnorderedList(ListItemStyle::Lines, {
                MakeListItem({ MakeParagraph(L"subitem") }),
            }),
            MakeParagraph(L"paragraph")
        }),
    }));

    ASSERT_TRUE(test(L"* item1\n\n    - subitem\n* item2", ListItemStyle::Blocks, {
        MakeListItem({
            MakeParagraph(L"item1"),
            MakeUnorderedList(ListItemStyle::Lines, {
                MakeListItem({ MakeParagraph(L"subitem") }),
            }),
        }),
        MakeListItem({ MakeParagraph(L"item2")}),
    }));
}


TEST(MarkdownParserTest, ParseOrderedList) {

    auto test = [](std::wstring_view input, std::size_t first_number, ElementList expected) {
        return TestParser(input, { 
            MakeOrderedList(ListItemStyle::Lines, first_number, std::move(expected)) 
        });
    };

    ASSERT_TRUE(test(L"1. item1\n2. item2\n3. item3", 1, {
        MakeListItem({ MakeParagraph(L"item1")}),
        MakeListItem({ MakeParagraph(L"item2")}),
        MakeListItem({ MakeParagraph(L"item3")}),
    }));

    //Spaces at the head of lines.
    ASSERT_TRUE(test(L" 1. item1\n  2. item2\n   3. item3", 1, {
        MakeListItem({ MakeParagraph(L"item1")}),
        MakeListItem({ MakeParagraph(L"item2")}),
        MakeListItem({ MakeParagraph(L"item3")}),
    }));

    ASSERT_TRUE(test(L"5. item1\n6. item2\n7. item3", 5, {
        MakeListItem({ MakeParagraph(L"item1")}),
        MakeListItem({ MakeParagraph(L"item2")}),
        MakeListItem({ MakeParagraph(L"item3")}),
    }));

    //Code block in list
    ASSERT_TRUE(test(
LR"(1. code
   ```
   void Function() {
       int x = 0;
   }
   ```
2. line)", 
        1,
        {
            MakeListItem({
                MakeParagraph(L"code"),
                MakeCodeBlock(L"void Function() {\n    int x = 0;\n}"),
            }),
            MakeListItem({ MakeParagraph(L"line")}),
        }));
}


TEST(MarkdownParserTest, ParseListFollowedByCodeBlock) {

    auto input =
LR"(1. code
```
function
```)";

    ASSERT_TRUE(TestParser(input, {
        MakeOrderedList(ListItemStyle::Lines, 1, { 
            MakeListItem({ MakeParagraph(L"code") })
        }),
        MakeCodeBlock(L"function"),
    }));
}