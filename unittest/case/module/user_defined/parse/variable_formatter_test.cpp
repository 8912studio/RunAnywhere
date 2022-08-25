#include <gtest/gtest.h>
#include <zaf/base/registry/registry.h>
#include "module/user_defined/parse/variable_formatter.h"

using namespace ra::context;
using namespace ra::module::user_defined;

TEST(VariableFormatterTest, FormatBadVariable) {

    auto bundle_meta = std::make_shared<BundleMeta>();
    VariableFormatter formatter{ bundle_meta, ActivePath{} };

    auto result = formatter.Format(L"{}", {});
    ASSERT_EQ(result, L"{}");

    result = formatter.Format(L"{", {});
    ASSERT_EQ(result, L"{");

    result = formatter.Format(L"}", {});
    ASSERT_EQ(result, L"}");

    result = formatter.Format(L"{  abc }", {});
    ASSERT_EQ(result, L"{  abc }");
}


TEST(VariableFormatterTest, FormatActivePath) {

    auto bundle_meta = std::make_shared<BundleMeta>();

    std::filesystem::path current_path{ __FILE__ };
    ActivePath active_path{ current_path, current_path.parent_path().parent_path() };

    VariableFormatter formatter{ bundle_meta, active_path };
    auto result = formatter.Format(L"{@}", {});
    ASSERT_EQ(result, active_path.GetPath());

    result = formatter.Format(L"{@~}", {});
    ASSERT_EQ(result, active_path.GetWorkspacePath());

    result = formatter.Format(L"{@.}", {});
    ASSERT_EQ(result, active_path.GetPath().parent_path());

    result = formatter.Format(L"{@..}", {});
    ASSERT_EQ(result, active_path.GetPath().parent_path().parent_path());
}


TEST(VariableFormatterTest, FormatFileVariable) {

    BundleMeta::Builder bundle_meta_builder;
    bundle_meta_builder.AddGlobalProperty(L"ExistentFile", __FILEW__);
    bundle_meta_builder.AddGlobalProperty(L"InexistentFile", L"SomeFile");

    VariableFormatter formatter{ bundle_meta_builder.Build(), ActivePath{} };

    auto result = formatter.Format(L"{ExistentFile}", {});
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{ExistentFile!}", {});
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{ExistentFile?}", {});
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{ExistentFile.}", {});
    ASSERT_EQ(result, std::filesystem::path(__FILEW__).parent_path());

    result = formatter.Format(L"{InexistentFile}", {});
    ASSERT_EQ(result, L"");

    result = formatter.Format(L"{InexistentFile!}", {});
    ASSERT_EQ(result, L"SomeFile");

    result = formatter.Format(L"{InexistentFile?}", {});
    ASSERT_EQ(result, L"SomeFile");

    result = formatter.Format(L"{InexistentFile.}", {});
    ASSERT_EQ(result, L"");
}


TEST(VariableFormatterTest, FormatRegistryVariable) {

    const std::wstring RegistryKeyPath = LR"(Software\RunAnywhere\Unittest\VariableFormatterTest)";

    auto key = zaf::Registry::CurrentUser().OpenSubKey(
        RegistryKeyPath,
        zaf::RegistryRights::Write);

    key.SetStringValue(L"", __FILEW__);
    key.SetStringValue(L"GoodFile", __FILEW__);
    key.SetStringValue(L"BadFile", L"RegistryFile");
    key.SetDWordValue(L"DWord", 11);

    BundleMeta::Builder bundle_meta_builder;
    bundle_meta_builder.AddGlobalProperty(
        L"RegDefaultValue",
        L"HKCU\\" + RegistryKeyPath + L"@");
    bundle_meta_builder.AddGlobalProperty(
        L"RegExistentFile",
        L"HKCU\\" + RegistryKeyPath + L"@GoodFile");
    bundle_meta_builder.AddGlobalProperty(
        L"RegInexistentFile",
        L"HKCU\\" + RegistryKeyPath + L"@BadFile");
    bundle_meta_builder.AddGlobalProperty(
        L"RegNonString",
        L"HKCU\\" + RegistryKeyPath + L"@DWord");

    VariableFormatter formatter{bundle_meta_builder.Build(), ActivePath{} };

    auto result = formatter.Format(L"{RegExistentFile}", {});
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{RegExistentFile?}", {});
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{RegExistentFile.}", {});
    ASSERT_EQ(result, std::filesystem::path(__FILEW__).parent_path());

    result = formatter.Format(L"{RegExistentFile!}", {});
    ASSERT_EQ(result, L"HKCU\\" + RegistryKeyPath + L"@GoodFile");

    result = formatter.Format(L"{RegInexistentFile}", {});
    ASSERT_EQ(result, L"");

    result = formatter.Format(L"{RegInexistentFile!}", {});
    ASSERT_EQ(result, L"HKCU\\" + RegistryKeyPath + L"@BadFile");

    result = formatter.Format(L"{RegInexistentFile?}", {});
    ASSERT_EQ(result, L"RegistryFile");

    result = formatter.Format(L"{RegInexistentFile.}", {});
    ASSERT_EQ(result, L"");

    result = formatter.Format(L"{RegNonString}", {});
    ASSERT_EQ(result, L"");

    result = formatter.Format(L"{RegNonString!}", {});
    ASSERT_EQ(result, L"HKCU\\" + RegistryKeyPath + L"@DWord");

    result = formatter.Format(L"{RegNonString?}", {});
    ASSERT_EQ(result, L"");
}


TEST(VariableFormatterTest, FormatMultiNamesVariable) {

    BundleMeta::Builder bundle_meta_builder;
    bundle_meta_builder.AddGlobalProperty(L"File", L"File1");
    bundle_meta_builder.AddGlobalProperty(L"File", L"File2");
    bundle_meta_builder.AddGlobalProperty(L"File", __FILEW__);

    VariableFormatter formatter{ bundle_meta_builder.Build(), ActivePath{} };

    auto result = formatter.Format(L"{File}", {});
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{File!}", {});
    ASSERT_EQ(result, L"File1");

    result = formatter.Format(L"{File?}", {});
    ASSERT_EQ(result, L"File1");
}


TEST(VariableFormatterTest, FormatMultiVariables) {

    BundleMeta::Builder bundle_meta_builder;
    bundle_meta_builder.AddGlobalProperty(L"V1", L"v1");
    bundle_meta_builder.AddGlobalProperty(L"V2", L"v2");
    bundle_meta_builder.AddGlobalProperty(L"V3", L"v3");

    VariableFormatter formatter{ bundle_meta_builder.Build(), ActivePath{} };

    auto result = formatter.Format(L"this is {V1!}{V2!} and {V3!} ", {});
    ASSERT_EQ(result, L"this is v1v2 and v3 ");
}