#include <gtest/gtest.h>
#include "module/user_defined/user_defined_bundle_parser.h"

using namespace ra::module::user_defined;

namespace {

std::filesystem::path GetInputFilePath(const std::string& file_name) {
    std::filesystem::path path(__FILE__);
    return path.parent_path() / "input" / file_name;
}


TEST(UserDefinedBundleParserTest, Case1) {

    auto input_path = GetInputFilePath("parser_test_1.rabdl");
    UserDefinedBundleParser parser(input_path);
    auto bundle = parser.Parse();
    ASSERT_NE(bundle, nullptr);

    ASSERT_EQ(bundle->Meta()->BundleID(), input_path.stem());
    
    const auto& entries = bundle->Entries();
    ASSERT_EQ(entries.size(), 2);
    
    ASSERT_EQ(entries[0]->Keyword(), L"log");
    ASSERT_EQ(
        entries[0]->Command(), 
        LR"("C:\Program Files\TortoiseGit\bin\TortoiseGitProc.exe" /command:log /path:%@)");

    ASSERT_EQ(entries[1]->Keyword(), L"cmd");
    ASSERT_EQ(entries[1]->Command(), LR"(C:\Windows\System32\cmd.exe /k "cd %.@")");
}


TEST(UserDefinedBundleParseTest, Case2) {

    auto input_path = GetInputFilePath("parser_test_2.rabdl");
    UserDefinedBundleParser parser(input_path);

    try {
        parser.Parse();
        GTEST_FAIL();
    }
    catch (const zaf::Error& error) {
        ASSERT_EQ(error.Code(), std::io_errc::stream);
    }
}


TEST(UserDefinedBundleParseTest, Case3) {

    auto input_path = GetInputFilePath("parser_test_3.rabdl");
    UserDefinedBundleParser parser(input_path);

    try {
        parser.Parse();
        GTEST_FAIL();
    }
    catch (const UserDefinedBundleParser::ParseError& error) {
        ASSERT_EQ(error.Code(), zaf::BasicErrc::InvalidValue);
        ASSERT_EQ(error.ErrorLineNumber(), 5);
        ASSERT_EQ(error.ErrorLine(), "error line");
    }
}

}