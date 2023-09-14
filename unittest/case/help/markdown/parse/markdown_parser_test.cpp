#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <zaf/base/string/encoding_conversion.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/markdown_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

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
            MakeText(L"Example:"),
        }),
        MakeCodeBlock(L"hex `32 ~16\nhex \"string to display\" /u8"),
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
        MakeItalics({
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