#include <gtest/gtest.h>
#include "module/tool/hash/sha_algorithm_type_parsing.h"

using namespace ra::mod::tool::hash;
using namespace ra::utility;

TEST(SHAAlgorithmTypeParsingTest, ParseSHAAlgorithmType) {

    {
        CommandLine command_line(L"sha");
        ASSERT_FALSE(ParseSHAAlgorithmType(command_line).has_value());
    }

    {
        CommandLine command_line(L"sha /f");
        ASSERT_FALSE(ParseSHAAlgorithmType(command_line).has_value());
    }

    {
        CommandLine command_line(L"sha /2");
        ASSERT_FALSE(ParseSHAAlgorithmType(command_line).has_value());
    }

    {
        CommandLine command_line(L"sha /1");
        ASSERT_EQ(ParseSHAAlgorithmType(command_line), SHAAlgorithmType::SHA1);
    }

    {
        CommandLine command_line(L"sha /256");
        ASSERT_EQ(ParseSHAAlgorithmType(command_line), SHAAlgorithmType::SHA256);
    }

    {
        CommandLine command_line(L"sha /384");
        ASSERT_EQ(ParseSHAAlgorithmType(command_line), SHAAlgorithmType::SHA384);
    }

    {
        CommandLine command_line(L"sha /512");
        ASSERT_EQ(ParseSHAAlgorithmType(command_line), SHAAlgorithmType::SHA512);
    }

    //The later has priority.
    {
        CommandLine command_line(L"sha /512 /1");
        ASSERT_EQ(ParseSHAAlgorithmType(command_line), SHAAlgorithmType::SHA1);
    }
}