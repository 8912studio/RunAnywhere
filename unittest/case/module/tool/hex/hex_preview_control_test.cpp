#include <gtest/gtest.h>
#include <zaf/base/handle.h>
#include "module/tool/hex/hex_preview_control.h"

using namespace ra::module::tool::hex;

namespace {

std::filesystem::path GetTestInputFilePath(const std::wstring& file_name) {
    return std::filesystem::path{ __FILEW__ }.parent_path() / file_name;
}

}

TEST(HexPreviewControlTest, ReadFileContent) {

    {
        std::vector<std::byte> file_content;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            HexCommandParseResult{},
            file_content);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
        ASSERT_EQ(file_content.size(), 128);
        ASSERT_EQ(
            std::string(reinterpret_cast<const char*>(file_content.data()), file_content.size()),
            "~!@#$%^&*()_+abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXY"
            "Z01234567899876543210ZYWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkji"
        );
    }

    {
        HexCommandParseResult parse_result;
        parse_result.position = 100;
        parse_result.length = 20;
        std::vector<std::byte> file_content;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            parse_result,
            file_content);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
        ASSERT_EQ(file_content.size(), 20);
        ASSERT_EQ(
            std::string(reinterpret_cast<const char*>(file_content.data()), file_content.size()),
            "JIHGFEDCBAzyxwvutsrq"
        );
    }
}


TEST(HexPreviewControlTest, ReadFileContent_ExceedsFileLength) {

    {
        HexCommandParseResult parse_result;
        parse_result.length = 300;
        std::vector<std::byte> file_content;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            parse_result,
            file_content);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
        ASSERT_EQ(file_content.size(), 149);
        ASSERT_EQ(
            std::string(reinterpret_cast<const char*>(file_content.data()), file_content.size()),
            "~!@#$%^&*()_+abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
            "9876543210ZYWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba+_)(*&^%$#@!~"
        );
    }

    {
        HexCommandParseResult parse_result;
        parse_result.position = 100;
        parse_result.length = 128;
        std::vector<std::byte> file_content;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            parse_result,
            file_content);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
        ASSERT_EQ(file_content.size(), 49);
        ASSERT_EQ(
            std::string(reinterpret_cast<const char*>(file_content.data()), file_content.size()),
            "JIHGFEDCBAzyxwvutsrqponmlkjihgfedcba+_)(*&^%$#@!~"
        );
    }
}


TEST(HexPreviewControlTest, ReadFileContent_EmptyFile) {

    {
        HexCommandParseResult parse_result;
        parse_result.position = 0;
        parse_result.length = 1;
        std::vector<std::byte> file_content;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file2.txt"),
            parse_result,
            file_content);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::NoContent);
        ASSERT_EQ(file_content.size(), 0);
    }

    {
        HexCommandParseResult parse_result;
        parse_result.position = 10;
        parse_result.length = 1;
        std::vector<std::byte> file_content;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file2.txt"),
            parse_result,
            file_content);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::NoContent);
        ASSERT_EQ(file_content.size(), 0);
    }
}


TEST(HexPreviewControlTest, ReadFileContent_NotFile) {

    std::vector<std::byte> file_content;
    auto status = HexPreviewControl::ReadFileContent(
        std::filesystem::path{ __FILEW__ }.parent_path(),
        HexCommandParseResult{},
        file_content);
    ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::Error);
    ASSERT_EQ(file_content.size(), 0);
}


TEST(HexPreviewControlTest, ReadFileContent_CannotOpenFile) {

    auto input_file = GetTestInputFilePath(L"hex_test_file1.txt");

    zaf::Handle file_handle{ CreateFile(
        input_file.c_str(),
        GENERIC_READ,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr) };

    ASSERT_TRUE(file_handle.IsValid());

    std::vector<std::byte> file_content;
    auto status = HexPreviewControl::ReadFileContent(
        input_file,
        HexCommandParseResult{},
        file_content);
    ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::Error);
    ASSERT_EQ(file_content.size(), 0);
}


TEST(HexPreviewControlTest, ReadFileContent_InvalidPosition) {

    {
        HexCommandParseResult parse_result;
        parse_result.position = 149;
        parse_result.length = 100;
        std::vector<std::byte> file_content;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            parse_result,
            file_content);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::InvalidPosition);
        ASSERT_EQ(file_content.size(), 0);
    }

    {
        HexCommandParseResult parse_result;
        parse_result.position = 200;
        parse_result.length = 100;
        std::vector<std::byte> file_content;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            parse_result,
            file_content);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::InvalidPosition);
        ASSERT_EQ(file_content.size(), 0);
    }
}