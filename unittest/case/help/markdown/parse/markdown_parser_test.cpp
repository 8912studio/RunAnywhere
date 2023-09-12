#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <zaf/base/string/encoding_conversion.h>
#include "help/markdown/element/factory.h"
#include "help/markdown/parse/markdown_parser.h"

using namespace ra::help::markdown::element;
using namespace ra::help::markdown::parse;

namespace {

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

TEST(MarkdownParserTest, Parse) {

    auto test = [](std::wstring_view filename, ElementList expected) {
        auto input = ReadTestFile(filename);
        auto element = MarkdownParser::Instance()->Parse(input);
        if (!element) {
            return false;
        }
        return element->IsEqualTo(*MakeRoot(std::move(expected)));
    };

    ASSERT_TRUE(test(L"markdown_parser_test_1.md", {
        MakeHeader(HeaderDepth::_1, L"Hex command help"),
        MakeParagraph(L"Display content of file in hex byte format."),
        MakeParagraph({
            MakeBold(L"Usage")
        }),
        MakeCodeBlock(L"hex [text] [position] [length] /f /u8 /u16"),
        MakeParagraph({
            MakeInlineCode(L"length"),
            MakeText(L" starts with "),
            MakeInlineCode(L"~"),
            MakeText(L", followed by a number."),
        }),
    }));
}