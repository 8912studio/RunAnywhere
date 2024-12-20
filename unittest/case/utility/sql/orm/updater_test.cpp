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


TEST_F(ORMQueryTest, WhereUpdater_Column) {

    auto& table = Entity::TableType::GetInstance();

    Entity entity;
    entity.id = 111;
    entity.name = "UPDATE";
    auto primitive_updater = EntitySet().BeginUpdate(entity);

    {
        auto changes = primitive_updater.Where(table.ID == 1).Execute();
        ASSERT_EQ(changes, 1);
        std::vector<Entity> expected{
            { 0, "9" },
            { 111, "UPDATE" },
            { 2, "7" },
            { 3, "6" },
            { 4, "5" },
        };
        ASSERT_EQ(EntitySet().SelectAll(), expected);
    }

    {
        auto changes = primitive_updater.Where(table.ID > 2 && table.ID < 10).Execute();
        ASSERT_EQ(changes, 2);
        std::vector<Entity> expected{
            { 0, "9" },
            { 111, "UPDATE" },
            { 2, "7" },
            { 111, "UPDATE" },
            { 111, "UPDATE" },
        };
        ASSERT_EQ(EntitySet().SelectAll(), expected);
    }
}


TEST_F(ORMQueryTest, WhereUpdater_PrimaryKey) {

    //Single column primary key
    {
        auto& table = EntityPK1::TableType::GetInstance();

        EntityPK1 entity;
        entity.id = 19;
        entity.name = "PK";
        auto changes = EntityPK1Set().BeginUpdate(entity).Where(table.PrimaryKey == 2).Execute();
        ASSERT_EQ(changes, 1);
        std::vector<EntityPK1> expected{
            { 0, "9" },
            { 1, "8" },
            { 3, "6" },
            { 4, "5" },
            { 19, "PK" },
        };
        ASSERT_EQ(EntityPK1Set().SelectAll(), expected);
    }

    //Multi-columns primary key
    {
        auto& table = EntityPK2::TableType::GetInstance();

        EntityPK2 entity;
        entity.id = 20;
        entity.name = "PK";
        auto changes = EntityPK2Set().BeginUpdate(entity)
            .Where(table.PrimaryKey == std::make_tuple(3, "6"))
            .Execute();
        ASSERT_EQ(changes, 1);
        std::vector<EntityPK2> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 20, "PK" },
            { 4, "5" },
        };
        auto actual = EntityPK2Set().SelectAll();
        ASSERT_EQ(actual, expected);
    }
}


TEST_F(ORMQueryTest, WhereUpdater_Index) {

    auto& table = Entity::TableType::GetInstance();

    Entity entity;
    entity.id = 101;
    entity.name = "INDEX";
    auto primitive_updater = EntitySet().BeginUpdate(entity);

    //Single column index
    {
        auto changes = primitive_updater.Where(table.IDIndex == 4).Execute();
        ASSERT_EQ(changes, 1);
        std::vector<Entity> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 3, "6" },
            { 101, "INDEX" },
        };
        ASSERT_EQ(EntitySet().SelectAll(), expected);
    }

    //Multi-columns index
    {
        auto changes = primitive_updater
            .Where(table.IDNameIndex == std::make_tuple(2, "7"))
            .Execute();
        ASSERT_EQ(changes, 1);
        std::vector<Entity> expected{
            { 0, "9" },
            { 1, "8" },
            { 101, "INDEX" },
            { 3, "6" },
            { 101, "INDEX" },
        };
        ASSERT_EQ(EntitySet().SelectAll(), expected);
    }
}

}