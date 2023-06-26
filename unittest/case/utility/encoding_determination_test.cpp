#include <gtest/gtest.h>
#include "utility/encoding_determination.h"

using namespace ra::utility;

namespace {

std::vector<std::byte> MakeData(std::u8string_view string) {
    std::vector<std::byte> result;
    for (auto ch : string) {
        result.push_back(static_cast<std::byte>(ch));
    }
    return result;
}

}

TEST(EncodingDeterminationTest, IsUTF8Encoded) {

    ASSERT_TRUE(IsUTF8Encoded(MakeData(u8"a")));
    ASSERT_TRUE(IsUTF8Encoded(MakeData(u8"abcdefg")));
    ASSERT_TRUE(IsUTF8Encoded(MakeData(u8"我")));
    ASSERT_TRUE(IsUTF8Encoded(MakeData(u8"对不起")));
}