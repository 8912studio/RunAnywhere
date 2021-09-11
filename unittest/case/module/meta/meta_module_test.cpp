#include <gtest/gtest.h>
#include "module/meta/meta_module.h"

using namespace ra::module::meta;

TEST(MetaModuleTest, QuerySuggestedCommands) {

    MetaModule meta_module;

    {
        auto result = meta_module.QuerySuggestedCommands(L"");
        ASSERT_FALSE(result.empty());
    }

    {
        auto result = meta_module.QuerySuggestedCommands(L"a");
        ASSERT_TRUE(result.empty());
    }

    {
        auto result = meta_module.QuerySuggestedCommands(L"!");
        ASSERT_FALSE(result.empty());
    }

    {
        auto result = meta_module.QuerySuggestedCommands(L"!ab");
        ASSERT_FALSE(result.empty());
    }

    {
        auto result = meta_module.QuerySuggestedCommands(L"!abdddddddddddd");
        ASSERT_TRUE(result.empty());
    }
}