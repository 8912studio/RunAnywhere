#include <gtest/gtest.h>
#include "module/user_defined/bundle_parser.h"

using namespace ra::module::user_defined;

namespace {

std::filesystem::path GetInputFilePath(const std::string& file_name) {
    std::filesystem::path path(__FILE__);
    return path.parent_path() / "bundle_parser_test_input" / file_name;
}


TEST(BundleParserTest, Case1) {

    auto input_path = GetInputFilePath("parser_test_1.ra-bundle");
    BundleParser parser(input_path);
    auto bundle = parser.Parse();
    ASSERT_NE(bundle, nullptr);

    auto meta = bundle->Meta();
    ASSERT_EQ(meta->BundleID(), input_path.stem());
    ASSERT_EQ(meta->GlobalProperties().size(), 2);
    ASSERT_EQ(meta->GlobalProperties()[0].first, L"AppPath");
    ASSERT_EQ(meta->GlobalProperties()[0].second, L"C:\\Windows\\System32\\kv.exe");
    ASSERT_EQ(meta->GlobalProperties()[1].first, L"AppReg");
    ASSERT_EQ(meta->GlobalProperties()[1].second, L"HKCU\\Software\\ff");
    
    const auto& entries = bundle->Entries();
    ASSERT_EQ(entries.size(), 2);
    
    ASSERT_EQ(entries[0]->Keyword(), L"log");
    ASSERT_EQ(
        entries[0]->Command(), 
        L"\"C:\\Program Files\\TortoiseGit\\bin\\TortoiseGitProc.exe\" /command:log /path:%@");

    ASSERT_EQ(entries[1]->Keyword(), L"cmd");
    ASSERT_EQ(entries[1]->Command(), L"C:\\Windows\\System32\\cmd.exe /k \"cd %.@\"");
    ASSERT_EQ(entries[1]->Description(), L"Open command line tool");
    ASSERT_EQ(entries[1]->WorkingDirectory(), L"D:\\RunAnywhere src");
}


TEST(BundleParseTest, Case2) {

    auto input_path = GetInputFilePath("parser_test_2.ra-bundle");
    BundleParser parser(input_path);

    try {
        parser.Parse();
        GTEST_FAIL();
    }
    catch (const zaf::Error& error) {
        ASSERT_EQ(error.Code(), std::io_errc::stream);
    }
}


TEST(BundleParseTest, Case3) {

    auto input_path = GetInputFilePath("parser_test_3.ra-bundle");
    BundleParser parser(input_path);

    try {
        parser.Parse();
        GTEST_FAIL();
    }
    catch (const BundleParser::ParseError& error) {
        ASSERT_EQ(error.Code(), zaf::BasicErrc::InvalidValue);
        ASSERT_EQ(error.ErrorLineNumber(), 5);
        ASSERT_EQ(error.ErrorLine(), "error line");
    }
}

}