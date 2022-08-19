#include <gtest/gtest.h>
#include <zaf/base/registry/registry.h>
#include "module/user_defined/parse/variable_formatter.h"

using namespace ra::context;
using namespace ra::module::user_defined;

TEST(VariableFormatterTest, FormatActivePath) {

    auto bundle_meta = std::make_shared<BundleMeta>();

    std::filesystem::path current_path{ __FILE__ };
    ActivePath active_path{ current_path, current_path.parent_path().parent_path() };

    VariableFormatter formatter{ bundle_meta, active_path };
    auto result = formatter.Format(L"{@}");
    ASSERT_EQ(result, active_path.GetPath());

    result = formatter.Format(L"{@~}");
    ASSERT_EQ(result, active_path.GetWorkspacePath());

    result = formatter.Format(L"{@.}");
    ASSERT_EQ(result, active_path.GetPath().parent_path());

    result = formatter.Format(L"{@..}");
    ASSERT_EQ(result, active_path.GetPath().parent_path().parent_path());
}


TEST(VariableFormatterTest, FormatGeneralVariable) {

    const std::wstring RegistryKeyPath = LR"(Software\RunAnywhere\Unittest\VariableFormatterTest)";

    zaf::Registry::CurrentUser().SetStringValue(RegistryKeyPath, L"", __FILEW__);
    zaf::Registry::CurrentUser().SetStringValue(RegistryKeyPath, L"GoodFile", __FILEW__);
    zaf::Registry::CurrentUser().SetStringValue(
        RegistryKeyPath,
        L"BadFile",
        L"RegistryFile");
    zaf::Registry::CurrentUser().SetDWordValue(RegistryKeyPath, L"DWord", 11);

    BundleMeta::Builder bundle_meta_builder;
    bundle_meta_builder.AddGlobalProperty(L"ExistentFile", __FILEW__);
    bundle_meta_builder.AddGlobalProperty(L"InexistentFile", L"SomeFile");
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

    VariableFormatter formatter{ 
        bundle_meta_builder.Build(), 
        ActivePath{}
    };

    auto result = formatter.Format(L"{ExistentFile}");
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{ExistentFile!}");
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{ExistentFile?}");
    ASSERT_EQ(result, __FILEW__);

    result = formatter.Format(L"{InexistentFile}");
    ASSERT_EQ(result, L"");

    result = formatter.Format(L"{InexistentFile!}");
    ASSERT_EQ(result, L"SomeFile");

    result = formatter.Format(L"{InexistentFile?}");
    ASSERT_EQ(result, L"SomeFile");

    result = formatter.Format(L"{RegExistentFile}");
    ASSERT_EQ(result, __FILEW__);
}