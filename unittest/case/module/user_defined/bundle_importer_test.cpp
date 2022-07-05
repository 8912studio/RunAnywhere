#include <gtest/gtest.h>
#include "module/user_defined/bundle_importer.h"
#include "module/user_defined/bundle_parser.h"

using namespace ra::module::user_defined;

namespace {

std::filesystem::path GetInputDirectoryPath() {
    std::filesystem::path path(__FILE__);
    return path.parent_path() / "bundle_importer_test_input";
}


std::filesystem::path GetTestingDepotDirectoryPath() {
    return GetInputDirectoryPath() / "testing_depot";
}


std::filesystem::path GetInputFilePath(const std::wstring& filename) {
    return GetInputDirectoryPath() / filename;
}


std::shared_ptr<BundleDepot> CreateDepot() {

    auto result = std::make_shared<BundleDepot>();

    for (std::filesystem::directory_iterator iterator(GetTestingDepotDirectoryPath());
         iterator != std::filesystem::directory_iterator();
         ++iterator) {

        BundleParser parser(iterator->path());
        result->AddBundle(parser.Parse());
    }
    
    return result;
}

}


class BundleImporterTest : public testing::Test {
protected:
    void SetUp() override {

        std::filesystem::remove_all(GetTestingDepotDirectoryPath());

        auto original_depot_path = GetInputDirectoryPath() / "original_depot";
        std::filesystem::copy(original_depot_path, GetTestingDepotDirectoryPath());
    }


    void TearDown() override {

        std::filesystem::remove_all(GetTestingDepotDirectoryPath());
    }
};


TEST_F(BundleImporterTest, Success) {

    auto depot = CreateDepot();

    BundleImporter importer(
        depot, 
        GetTestingDepotDirectoryPath(),
        GetInputFilePath(L"success.rabdl"));

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Pending);

    importer.Import();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Success);
    ASSERT_NE(depot->FindBundle(L"success"), nullptr);
    ASSERT_TRUE(std::filesystem::exists(GetTestingDepotDirectoryPath() / "success.rabdl"));
}


TEST_F(BundleImporterTest, Override) {

    const auto bundle_in_depot = GetTestingDepotDirectoryPath() / "override.rabdl";
    std::filesystem::rename(GetTestingDepotDirectoryPath() / "default.rabdl", bundle_in_depot);

    auto depot = CreateDepot();

    BundleImporter importer(
        depot, 
        GetTestingDepotDirectoryPath(),
        GetInputFilePath(L"override.rabdl"));

    importer.Import();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::OverrideConfirm);

    auto bundle = depot->FindBundle(L"override");
    ASSERT_NE(bundle, nullptr);
    ASSERT_EQ(bundle->Entries().size(), 2);
    ASSERT_EQ(bundle->Entries()[0]->Keyword(), L"a");
    ASSERT_EQ(bundle->Entries()[1]->Keyword(), L"b");
    ASSERT_EQ(std::filesystem::file_size(bundle_in_depot), 34);

    importer.Confirm();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Success);

    bundle = depot->FindBundle(L"override");
    ASSERT_NE(bundle, nullptr);
    ASSERT_EQ(bundle->Entries().size(), 3);
    ASSERT_EQ(bundle->Entries()[0]->Keyword(), L"c");
    ASSERT_EQ(bundle->Entries()[1]->Keyword(), L"d");
    ASSERT_EQ(bundle->Entries()[2]->Keyword(), L"e");
    ASSERT_EQ(std::filesystem::file_size(bundle_in_depot), 53);
}


TEST_F(BundleImporterTest, Conflict) {

    auto depot = CreateDepot();

    BundleImporter importer(
        depot,
        GetTestingDepotDirectoryPath(),
        GetInputFilePath(L"conflict.rabdl"));

    importer.Import();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::ConflictConfirm);

    auto bundle = depot->FindBundle(L"conflict");
    ASSERT_EQ(bundle, nullptr);
    ASSERT_FALSE(std::filesystem::exists(GetTestingDepotDirectoryPath() / "conflict.rabdl"));

    const auto& conflict_entries = importer.GetConflictEntries();
    ASSERT_EQ(conflict_entries.size(), 2);
    ASSERT_EQ(conflict_entries[0]->Keyword(), L"a");
    ASSERT_EQ(conflict_entries[1]->Keyword(), L"b");

    importer.Confirm();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Success);

    bundle = depot->FindBundle(L"conflict");
    ASSERT_NE(bundle, nullptr);
    ASSERT_EQ(bundle->Entries().size(), 2);
    ASSERT_EQ(bundle->Entries()[0]->Keyword(), L"a");
    ASSERT_EQ(bundle->Entries()[1]->Keyword(), L"b");
    ASSERT_TRUE(std::filesystem::exists(GetTestingDepotDirectoryPath() / "conflict.rabdl"));
}


TEST_F(BundleImporterTest, CannotOpenFile) {

    auto depot = CreateDepot();

    BundleImporter importer(
        depot,
        GetTestingDepotDirectoryPath(),
        GetInputFilePath(L"inexistent.rabdl"));

    importer.Import();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Fail);
    ASSERT_EQ(importer.GetFailReason(), BundleImporter::FailReason::CannotOpenFile);
    ASSERT_EQ(depot->FindBundle(L"inexistent"), nullptr);
    ASSERT_FALSE(std::filesystem::exists(GetTestingDepotDirectoryPath() / "inexistent.rabdl"));
}


TEST_F(BundleImporterTest, ParseError) {

    auto depot = CreateDepot();

    BundleImporter importer(
        depot,
        GetTestingDepotDirectoryPath(),
        GetInputFilePath(L"parse_error.rabdl"));

    importer.Import();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Fail);
    ASSERT_EQ(importer.GetFailReason(), BundleImporter::FailReason::ParseError);
    ASSERT_NE(importer.GetParseError(), nullptr);
    ASSERT_EQ(importer.GetParseError()->ErrorLine(), "Command*");
    ASSERT_EQ(importer.GetParseError()->ErrorLineNumber(), 5);
    ASSERT_EQ(depot->FindBundle(L"parse_error"), nullptr);
    ASSERT_FALSE(std::filesystem::exists(GetTestingDepotDirectoryPath() / "parse_error.rabdl"));
}


TEST_F(BundleImporterTest, NeedRetry) {

    auto depot = CreateDepot();

    BundleImporter importer(
        depot,
        GetTestingDepotDirectoryPath(),
        GetInputFilePath(L"success.rabdl"));

    depot.reset();

    importer.Import();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Fail);
    ASSERT_EQ(importer.GetFailReason(), BundleImporter::FailReason::NeedRetry);
    ASSERT_FALSE(std::filesystem::exists(GetTestingDepotDirectoryPath() / "success.rabdl"));
}


//Bundle is in depot, but file is missing.
TEST_F(BundleImporterTest, NeedReload1) {

    const auto bundle_file_in_depot = GetTestingDepotDirectoryPath() / "need_reload1.rabdl";
    std::filesystem::rename(GetTestingDepotDirectoryPath() / "default.rabdl", bundle_file_in_depot);

    auto depot = CreateDepot();
    std::filesystem::remove(bundle_file_in_depot);

    BundleImporter importer(
        depot,
        GetTestingDepotDirectoryPath(),
        GetInputFilePath(L"need_reload1.rabdl"));

    importer.Import();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Fail);
    ASSERT_EQ(importer.GetFailReason(), BundleImporter::FailReason::NeedReload);
    ASSERT_NE(depot->FindBundle(L"need_reload1"), nullptr);
    ASSERT_FALSE(std::filesystem::exists(bundle_file_in_depot));
}


//Bundle is not in depot, but file exists.
TEST_F(BundleImporterTest, NeedReload2) {

    auto depot = CreateDepot();

    const auto bundle_file_in_depot = GetTestingDepotDirectoryPath() / "need_reload2.rabdl";
    std::filesystem::rename(GetTestingDepotDirectoryPath() / "default.rabdl", bundle_file_in_depot);

    BundleImporter importer(
        depot,
        GetTestingDepotDirectoryPath(),
        GetInputFilePath(L"need_reload2.rabdl"));

    importer.Import();

    ASSERT_EQ(importer.GetState(), BundleImporter::State::Fail);
    ASSERT_EQ(importer.GetFailReason(), BundleImporter::FailReason::NeedReload);
    ASSERT_EQ(depot->FindBundle(L"need_reload2"), nullptr);
    ASSERT_TRUE(std::filesystem::exists(bundle_file_in_depot));
}