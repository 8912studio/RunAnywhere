#include <gtest/gtest.h>
#include <zaf/base/handle.h>
#include "module/tool/hex/hex_preview_control.h"

using namespace ra::mod::tool::hex;

namespace {

std::filesystem::path GetTestInputFilePath(const std::wstring& file_name) {
    return std::filesystem::path{ __FILEW__ }.parent_path() / file_name;
}

}

TEST(HexPreviewControlTest, ReadFileContent) {

    {
        HexPreviewControl::FileContentInfo content_info;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            zaf::Range{ 0, HexCommandParseResult::DefaultLength },
            content_info);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
        ASSERT_EQ(content_info.data.size(), 128);
        ASSERT_EQ(content_info.file_size, 149);
        ASSERT_EQ(
            std::string(reinterpret_cast<const char*>(
                content_info.data.data()), content_info.data.size()),
            "~!@#$%^&*()_+abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXY"
            "Z01234567899876543210ZYWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkji"
        );
    }

    {
        HexPreviewControl::FileContentInfo content_info;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            zaf::Range{ 100, 20 },
            content_info);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
        ASSERT_EQ(content_info.data.size(), 20);
        ASSERT_EQ(content_info.file_size, 149);
        ASSERT_EQ(
            std::string(reinterpret_cast<const char*>(
                content_info.data.data()), content_info.data.size()),
            "JIHGFEDCBAzyxwvutsrq"
        );
    }
}


TEST(HexPreviewControlTest, ReadFileContent_ExceedsFileLength) {

    {
        HexPreviewControl::FileContentInfo content_info;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            zaf::Range{ 0, 300 },
            content_info);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
        ASSERT_EQ(content_info.data.size(), 149);
        ASSERT_EQ(content_info.file_size, 149);
        ASSERT_EQ(
            std::string(reinterpret_cast<const char*>(content_info.data.data()), content_info.data.size()),
            "~!@#$%^&*()_+abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
            "9876543210ZYWVUTSRQPONMLKJIHGFEDCBAzyxwvutsrqponmlkjihgfedcba+_)(*&^%$#@!~"
        );
    }

    {
        HexCommandParseResult parse_result;
        parse_result.position = 100;
        parse_result.length = 128;
        HexPreviewControl::FileContentInfo content_info;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            zaf::Range{ 100, 128 },
            content_info);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
        ASSERT_EQ(content_info.data.size(), 49);
        ASSERT_EQ(content_info.file_size, 149);
        ASSERT_EQ(
            std::string(reinterpret_cast<const char*>(content_info.data.data()), content_info.data.size()),
            "JIHGFEDCBAzyxwvutsrqponmlkjihgfedcba+_)(*&^%$#@!~"
        );
    }
}


TEST(HexPreviewControlTest, ReadFileContent_EmptyFile) {

    {
        HexPreviewControl::FileContentInfo content_info;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file2.txt"),
            zaf::Range{ 0, 1 },
            content_info);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::EmptyFile);
        ASSERT_EQ(content_info.data.size(), 0);
        ASSERT_EQ(content_info.file_size, 0);
    }

    {
        HexPreviewControl::FileContentInfo content_info;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file2.txt"),
            zaf::Range{ 10, 1 },
            content_info);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::EmptyFile);
        ASSERT_EQ(content_info.data.size(), 0);
        ASSERT_EQ(content_info.file_size, 0);
    }
}


TEST(HexPreviewControlTest, ReadFileContent_NotFile) {

    HexPreviewControl::FileContentInfo content_info;
    auto status = HexPreviewControl::ReadFileContent(
        std::filesystem::path{ __FILEW__ }.parent_path(),
        zaf::Range{ 0, HexCommandParseResult::DefaultLength },
        content_info);
    ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::ReadFileFailed);
    ASSERT_EQ(content_info.data.size(), 0);
    ASSERT_EQ(content_info.file_size, 0);
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

    HexPreviewControl::FileContentInfo content_info;
    auto status = HexPreviewControl::ReadFileContent(
        input_file,
        zaf::Range{ 0, HexCommandParseResult::DefaultLength },
        content_info);
    ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::ReadFileFailed);
    ASSERT_EQ(content_info.data.size(), 0);
    ASSERT_EQ(content_info.file_size, 0);
}


TEST(HexPreviewControlTest, ReadFileContent_InvalidPosition) {

    {
        HexPreviewControl::FileContentInfo content_info;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            zaf::Range{ 149, 100 },
            content_info);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::InvalidPosition);
        ASSERT_EQ(content_info.data.size(), 0);
        ASSERT_EQ(content_info.file_size, 149);
    }

    {
        HexPreviewControl::FileContentInfo content_info;
        auto status = HexPreviewControl::ReadFileContent(
            GetTestInputFilePath(L"hex_test_file1.txt"),
            zaf::Range{ 200, 100 },
            content_info);
        ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::InvalidPosition);
        ASSERT_EQ(content_info.data.size(), 0);
        ASSERT_EQ(content_info.file_size, 149);
    }
}


TEST(HexPreviewControlTest, ReadFileContent_ZeroLength) {

    HexPreviewControl::FileContentInfo content_info;
    auto status = HexPreviewControl::ReadFileContent(
        GetTestInputFilePath(L"hex_test_file1.txt"),
        zaf::Range{},
        content_info);
    ASSERT_EQ(status, HexPreviewControl::ReadFileStatus::OK);
    ASSERT_EQ(content_info.data.size(), 128);
    ASSERT_EQ(content_info.file_size, 149);
}