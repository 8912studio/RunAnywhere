#include <gtest/gtest.h>
#include "module/crypto/md5/md5_command_parsing.h"

using namespace ra::module::crypto;

TEST(MD5CommandParsingTest, ParseSucceeded) {

    {
        ra::utility::CommandLine command_line(L"md5");
        auto result = ParseMD5Command(command_line);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result->string, std::wstring{});
        ASSERT_EQ(result->use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 \"");
        auto result = ParseMD5Command(command_line);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result->string, std::wstring{});
        ASSERT_EQ(result->use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 \"what is this\"");
        auto result = ParseMD5Command(command_line);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result->string, L"what is this");
        ASSERT_EQ(result->use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 help /u8");
        auto result = ParseMD5Command(command_line);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result->string, L"help");
        ASSERT_EQ(result->use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 help /u16");
        auto result = ParseMD5Command(command_line);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->encoding, MD5Encoding::UTF16);
        ASSERT_EQ(result->string, L"help");
        ASSERT_EQ(result->use_uppercase, false);
    }

    {
        ra::utility::CommandLine command_line(L"md5 help /c");
        auto result = ParseMD5Command(command_line);
        ASSERT_TRUE(result.has_value());
        ASSERT_EQ(result->encoding, MD5Encoding::UTF8);
        ASSERT_EQ(result->string, L"help");
        ASSERT_EQ(result->use_uppercase, true);
    }
}


TEST(MD5CommandParsingTest, ParseFailed) {

    {
        ra::utility::CommandLine command_line(L"");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.has_value());
    }

    {
        ra::utility::CommandLine command_line(L"md55");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.has_value());
    }

    {
        ra::utility::CommandLine command_line(L"abc");
        auto result = ParseMD5Command(command_line);
        ASSERT_FALSE(result.has_value());
    }
}