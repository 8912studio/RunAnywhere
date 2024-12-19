#include <gtest/gtest.h>
#include <zaf/base/range.h>
#include "orm_query_test.h"
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"

namespace ra::test {

TEST_F(ORMQueryTest, SelectAll) {

    auto result = EntitySet().SelectAll();
    std::vector<Entity> expected{
        { 0, "9" },
        { 1, "8" },
        { 2, "7" },
        { 3, "6" },
        { 4, "5" },
    };
    ASSERT_EQ(result, expected);
}


TEST_F(ORMQueryTest, SelectByPrimaryKey) {

    auto result = EntityPK1Set().Select(1);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->id, 1);
    ASSERT_EQ(result->name, "8");

    result = EntityPK1Set().Select(5);
    ASSERT_FALSE(result.has_value());
}

}