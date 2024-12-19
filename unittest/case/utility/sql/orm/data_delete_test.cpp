#include <gtest/gtest.h>
#include "orm_query_test.h"

using namespace ra::utility::sql;

namespace ra::test {

TEST_F(ORMQueryTest, DeleteByPrimaryKey) {

    //Single column primary key
    {
        auto changes = EntityPK1Set().Delete(5);
        ASSERT_EQ(changes, 0);

        changes = EntityPK1Set().Delete(1);
        ASSERT_EQ(changes, 1);

        std::vector<EntityPK1> expected{
            { 0, "9" },
            { 2, "7" },
            { 3, "6" },
            { 4, "5" },
        };
        auto actual = EntityPK1Set().SelectAll();
        ASSERT_EQ(actual, expected);
    }
    
    //Multiple columns primary key
    {
        auto changes = EntityPK2Set().Delete(std::make_tuple(4, "4"));
        ASSERT_EQ(changes, 0);

        changes = EntityPK2Set().Delete(std::make_tuple(4, "5"));
        ASSERT_EQ(changes, 1);

        std::vector<EntityPK2> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 3, "6" },
        };
        auto actual = EntityPK2Set().SelectAll();
        ASSERT_EQ(actual, expected);
    }
}

}