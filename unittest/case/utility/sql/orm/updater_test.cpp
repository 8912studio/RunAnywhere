#include <gtest/gtest.h>
#include "orm_query_test.h"

namespace ra::test {

TEST_F(ORMQueryTest, PrimitiveUpdater_Entity) {

    Entity entity;
    entity.id = 11;
    entity.name = "ee";
    auto changes = EntitySet().BeginUpdate(entity).Execute();
    ASSERT_EQ(changes, 5);

    std::vector<Entity> expected{
        { 11, "ee" },
        { 11, "ee" },
        { 11, "ee" },
        { 11, "ee" },
        { 11, "ee" },
    };
    ASSERT_EQ(EntitySet().SelectAll(), expected);
}


TEST_F(ORMQueryTest, PrimitiveUpdater_Column) {

    auto& table = Entity::TableType::GetInstance();

    //Update single column
    {
        auto changes = EntitySet().BeginUpdate(table.Name = "zu").Execute();
        ASSERT_EQ(changes, 5);
        std::vector<Entity> expected{
            { 0, "zu" },
            { 1, "zu" },
            { 2, "zu" },
            { 3, "zu" },
            { 4, "zu" },
        };
        ASSERT_EQ(EntitySet().SelectAll(), expected);
    }

    //Update multi-columns
    {
        auto changes = EntitySet().BeginUpdate(table.ID = 10, table.Name = "ten").Execute();
        ASSERT_EQ(changes, 5);
        std::vector<Entity> expected{
            { 10, "ten" },
            { 10, "ten" },
            { 10, "ten" },
            { 10, "ten" },
            { 10, "ten" },
        };
        ASSERT_EQ(EntitySet().SelectAll(), expected);
    }

    //Update single column index
    {
        auto changes = EntitySet().BeginUpdate(table.IDIndex = 21).Execute();
        ASSERT_EQ(changes, 5);
        std::vector<Entity> expected{
            { 21, "ten" },
            { 21, "ten" },
            { 21, "ten" },
            { 21, "ten" },
            { 21, "ten" },
        };
        ASSERT_EQ(EntitySet().SelectAll(), expected);
    }

    //Update multi-columns index
    {
        auto changes = EntitySet()
            .BeginUpdate(table.IDNameIndex = std::make_tuple(33, "th"))
            .Execute();
        ASSERT_EQ(changes, 5);
        std::vector<Entity> expected{
            { 33, "th" },
            { 33, "th" },
            { 33, "th" },
            { 33, "th" },
            { 33, "th" },
        };
        ASSERT_EQ(EntitySet().SelectAll(), expected);
    }
}

}