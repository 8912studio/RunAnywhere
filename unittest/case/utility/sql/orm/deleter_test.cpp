#include <gtest/gtest.h>
#include "orm_query_test.h"
#include "utility/sql/orm/orm_support.h"

using namespace ra::utility::sql;

namespace ra::test {

TEST_F(ORMQueryTest, PrimitiveDeleter) {

    auto changes = EntitySet().BeginDelete().Execute();
    ASSERT_EQ(changes, 5);
    ASSERT_TRUE(EntitySet().SelectAll().empty());
}


TEST_F(ORMQueryTest, WhereDeleter_Column) {

    auto& table = Entity::TableType::GetInstance();

    auto primitive_deleter = EntitySet().BeginDelete();
    auto changes = primitive_deleter.Where(table.ID == 0).Execute();
    ASSERT_EQ(changes, 1);

    changes = primitive_deleter.Where(table.ID == 3 || table.Name == "7").Execute();
    ASSERT_EQ(changes, 2);

    std::vector<Entity> expected{
        { 1, "8" },
        { 4, "5" },
    };
    auto actual = EntitySet().SelectAll();
    ASSERT_EQ(actual, expected);
}


TEST_F(ORMQueryTest, WhereDeleter_PrimaryKey) {

    //Single column primary key
    {
        auto& table = Entity::TableType::GetInstance();

        auto changes = EntitySet().BeginDelete().Where(table.PrimaryKey == 4).Execute();
        ASSERT_EQ(changes, 1);

        std::vector<Entity> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 3, "6" },
        };
        auto actual = EntitySet().SelectAll();
        ASSERT_EQ(actual, expected);
    }

    //Multiple column primary key
    {
        auto& table = EntityPK2::TableType::GetInstance();

        auto changes = EntityPK2Set().BeginDelete()
            .Where(table.PrimaryKey == std::make_tuple(2, "7"))
            .Execute();
        ASSERT_EQ(changes, 1);

        std::vector<EntityPK2> expected{
            { 0, "9" },
            { 1, "8" },
            { 3, "6" },
            { 4, "5" },
        };
        auto actual = EntityPK2Set().SelectAll();
        ASSERT_EQ(actual, expected);
    }
}


TEST_F(ORMQueryTest, WhereDeleter_Index) {

    auto& table = Entity::TableType::GetInstance();
    auto primitive_deleter = EntitySet().BeginDelete();

    //Single column index
    {
        auto changes = primitive_deleter.Where(table.IDIndex == 3).Execute();
        ASSERT_EQ(changes, 1);

        std::vector<Entity> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 4, "5" },
        };
        auto actual = EntitySet().SelectAll();
        ASSERT_EQ(actual, expected);
    }

    //Multiple column index
    {
        auto changes = primitive_deleter
            .Where(table.IDNameIndex == std::make_tuple(4, "5"))
            .Execute();
        ASSERT_EQ(changes, 1);

        std::vector<Entity> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
        };
        auto actual = EntitySet().SelectAll();
        ASSERT_EQ(actual, expected);
    }
}

}