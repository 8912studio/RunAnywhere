#include <gtest/gtest.h>
#include "orm_query_test.h"
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"
#include "utility/sql/orm/select/primitive_selecter.h"
#include "utility/sql/orm/select/ordering_column.h"

using namespace ra::utility::sql;

namespace ra::test {

TEST_F(ORMQueryTest, SelectEntity) {

    auto& table = Entity::TableType::GetInstance();

    // Primitive
    {
        auto result = EntitySet().BeginSelect().Execute();
        std::vector<Entity> expected{
            Entity{ 0, "9" },
            Entity{ 1, "8" },
            Entity{ 2, "7" },
            Entity{ 3, "6" },
            Entity{ 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }

    // Where
    {
        auto selecter = EntitySet().BeginSelect().Where(table.ID == 2 || table.ID == 3);
        auto result = selecter.Execute();
        std::vector<Entity> expected{
            Entity{ 2, "7" },
            Entity{ 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // Where + OrderBy
    {
        auto where_selecter = EntitySet().BeginSelect().Where(
            table.ID == 1 || table.ID == 2 || table.ID == 3);
        auto order_by_selecter = where_selecter.OrderBy(table.Name);
        auto result = order_by_selecter.Execute();
        std::vector<Entity> expected{
            Entity{ 3, "6" },
            Entity{ 2, "7" },
            Entity{ 1, "8" },
        };
        ASSERT_EQ(result, expected);
    }

    // Where + OrderBy + Limit
    {
        auto where_selecter = EntitySet().BeginSelect().Where(
            table.ID == 1 || table.ID == 3);
        auto result = where_selecter.OrderBy(table.Name).Limit(1).Execute();
        std::vector<Entity> expected{
            Entity{ 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // OrderBy
    {
        auto result = EntitySet().BeginSelect().OrderBy(table.Name).Execute();
        std::vector<Entity> expected{
            Entity{ 4, "5" },
            Entity{ 3, "6" },
            Entity{ 2, "7" },
            Entity{ 1, "8" },
            Entity{ 0, "9" },
        };
        ASSERT_EQ(result, expected);
    }

    // OrderBy + Limit
    {
        auto result = EntitySet().BeginSelect().OrderBy(table.Name).Limit(2).Execute();
        std::vector<Entity> expected{
            Entity{ 4, "5" },
            Entity{ 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // Limit
    {
        auto result = EntitySet().BeginSelect().Limit(3).Execute();
        std::vector<Entity> expected{
            Entity{ 0, "9" },
            Entity{ 1, "8" },
            Entity{ 2, "7" },
        };
        ASSERT_EQ(result, expected);
    }
}


TEST_F(ORMQueryTest, SelectSingleColumn) {

    auto& table = Entity::TableType::GetInstance();
    auto primitive_selecter = EntitySet().BeginSelect(table.ID);

    // Primitive
    {
        auto result = primitive_selecter.Execute();
        std::vector<int> expected{ 0, 1, 2, 3, 4 };
        ASSERT_EQ(result, expected);
    }

    // Where
    {
        auto result = primitive_selecter.Where(table.ID == 4).Execute();
        std::vector<int> expected{ 4 };
        ASSERT_EQ(result, expected);
    }

    // Where + OrderBy
    {
        auto where_selecter = primitive_selecter.Where(table.ID == 1 || table.ID == 2);
        auto result = where_selecter.OrderBy(table.Name).Execute();
        std::vector<int> expected{ 2, 1 };
        ASSERT_EQ(result, expected);
    }

    // Where + OrderBy + Limit
    {
        auto where_selecter = primitive_selecter.Where(table.ID == 1 || table.ID == 2);
        auto result = where_selecter.OrderBy(table.Name).Limit(1).Execute();
        std::vector<int> expected{ 2 };
        ASSERT_EQ(result, expected);
    }

    // OrderBy
    {
        auto result = primitive_selecter.OrderBy(table.Name).Execute();
        std::vector<int> expected{ 4, 3, 2, 1, 0 };
        ASSERT_EQ(result, expected);
    }

    // OrderBy + Limit
    {
        auto result = primitive_selecter.OrderBy(table.Name).Limit(3).Execute();
        std::vector<int> expected{ 4, 3, 2 };
        ASSERT_EQ(result, expected);
    }

    // Limit
    {
        auto result = primitive_selecter.Limit(2).Execute();
        std::vector<int> expected{ 0, 1 };
        ASSERT_EQ(result, expected);
    }
}


TEST_F(ORMQueryTest, SelectMultipleColumns) {

    auto& table = Entity::TableType::GetInstance();
    auto primitive_selecter = EntitySet().BeginSelect(table.ID, table.Name);

    // Primitive
    {
        auto result = primitive_selecter.Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 3, "6" },
            { 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }

    // Where
    {
        auto result = primitive_selecter.Where(table.ID == 3).Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // Where + OrderBy
    {
        auto where_selecter = primitive_selecter.Where(table.ID == 3 || table.ID == 2);
        auto result = where_selecter.OrderBy(table.Name).Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 3, "6" },
            { 2, "7" },
        };
        ASSERT_EQ(result, expected);
    }

    // Where + OrderBy + Limit
    {
        auto where_selecter = primitive_selecter.Where(table.ID == 3 || table.ID == 2);
        auto result = where_selecter.OrderBy(table.Name).Limit(1).Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // OrderBy
    {
        auto result = primitive_selecter.OrderBy(table.Name).Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 4, "5" },
            { 3, "6" },
            { 2, "7" },
            { 1, "8" },
            { 0, "9" },
        };
        ASSERT_EQ(result, expected);
    }

    // OrderBy + Limit
    {
        auto result = primitive_selecter.OrderBy(table.Name).Limit(2).Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 4, "5" },
            { 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // Limit
    {
        auto result = primitive_selecter.Limit(4).Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }
}


TEST_F(ORMQueryTest, WhereSelecter_Column) {

    auto& table = Entity::TableType::GetInstance();
    auto primitive_selecter = EntitySet().BeginSelect();

    //Equal
    {
        std::vector<Entity> expected{
            { 2, "7" },
        };
        auto result = primitive_selecter.Where(table.ID == 2).Execute();
        ASSERT_EQ(result, expected);
        result = primitive_selecter.Where(2 == table.ID).Execute();
        ASSERT_EQ(result, expected);
    }

    //Not equal
    {
        std::vector<Entity> expected{
            { 0, "9" },
            { 1, "8" },
            { 3, "6" },
            { 4, "5" },
        };
        auto result = primitive_selecter.Where(table.ID != 2).Execute();
        ASSERT_EQ(result, expected);
        result = primitive_selecter.Where(2 != table.ID).Execute();
        ASSERT_EQ(result, expected);
    }

    //Less
    {
        auto result = primitive_selecter.Where(table.ID < 3).Execute();
        std::vector<Entity> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
        };
        ASSERT_EQ(result, expected);
        result = primitive_selecter.Where(3 < table.ID).Execute();
        expected = {
            { 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }

    //Less equal
    {
        auto result = primitive_selecter.Where(table.ID <= 2).Execute();
        std::vector<Entity> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
        };
        ASSERT_EQ(result, expected);
        result = primitive_selecter.Where(3 <= table.ID).Execute();
        expected = {
            { 3, "6" },
            { 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }

    //Greater
    {
        auto result = primitive_selecter.Where(table.ID > 3).Execute();
        std::vector<Entity> expected{
            { 4, "5" },
        };
        ASSERT_EQ(result, expected);
        result = primitive_selecter.Where(3 > table.ID).Execute();
        expected = {
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
        };
        ASSERT_EQ(result, expected);
    }

    //Greater equal
    {
        auto result = primitive_selecter.Where(table.ID >= 3).Execute();
        std::vector<Entity> expected{
            { 3, "6" },
            { 4, "5" },
        };
        ASSERT_EQ(result, expected);
        result = primitive_selecter.Where(3 >= table.ID).Execute();
        expected = {
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    //And
    {
        auto result = primitive_selecter.Where(2 < table.ID && table.ID < 4).Execute();
        std::vector<Entity> expected{
            { 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    //Or
    {
        auto result = primitive_selecter.Where(table.ID == 1 || table.ID >= 3).Execute();
        std::vector<Entity> expected{
            { 1, "8" },
            { 3, "6" },
            { 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }
}


TEST_F(ORMQueryTest, WhereSelecter_Index) {

    auto& table = Entity::TableType::GetInstance();
    auto primitive_selecter = EntitySet().BeginSelect();

    //Use with single column index
    {
        auto result = primitive_selecter.Where(table.IDIndex == 4).Execute();
        std::vector<Entity> expected{
            { 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }

    //Use with multiple column index
    {
        auto result = primitive_selecter.Where(table.IDNameIndex == std::make_tuple(0, "9"))
            .Execute();
        std::vector<Entity> expected{
            { 0, "9" },
        };
        ASSERT_EQ(result, expected);
    }
}


TEST_F(ORMQueryTest, WhereSelecter_PrimaryKey) {

    //Use with single column primary key
    {
        auto& table = EntityPK1::TableType::GetInstance();
        auto result = EntityPK1Set().BeginSelect().Where(table.PrimaryKey == 1).Execute();
        std::vector<EntityPK1> expected{
            { 1, "8" },
        };
        ASSERT_EQ(result, expected);
    }

    //Use with multiple columns primary key
    {
        auto& table = EntityPK2::TableType::GetInstance();
        auto result = EntityPK2Set().BeginSelect()
            .Where(table.PrimaryKey == std::make_tuple(3, "6")).Execute();
        std::vector<EntityPK2> expected{
            { 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }
}


TEST_F(ORMQueryTest, OrderBySelecter) {

    auto& table = Entity::TableType::GetInstance();

    //OrderBy using single column.
    {
        auto result = EntitySet().BeginSelect().OrderBy(table.ID).Execute();
        std::vector<Entity> expected{
            Entity{ 0, "9" },
            Entity{ 1, "8" },
            Entity{ 2, "7" },
            Entity{ 3, "6" },
            Entity{ 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }

    //OrderBy using multiple columns.
    {
        auto result = EntitySet().BeginSelect().OrderBy(table.Name, table.ID).Execute();
        std::vector<Entity> expected{
            Entity{ 4, "5" },
            Entity{ 3, "6" },
            Entity{ 2, "7" },
            Entity{ 1, "8" },
            Entity{ 0, "9" },
        };
        ASSERT_EQ(result, expected);
    }

    //OrderBy using ascending column.
    {
        auto result = EntitySet().BeginSelect().OrderBy(table.Name.Asc()).Execute();
        std::vector<Entity> expected{
            Entity{ 4, "5" },
            Entity{ 3, "6" },
            Entity{ 2, "7" },
            Entity{ 1, "8" },
            Entity{ 0, "9" },
        };
        ASSERT_EQ(result, expected);
    }

    //OrderBy using descending column.
    {
        auto result = EntitySet().BeginSelect().OrderBy(table.ID.Desc()).Execute();
        std::vector<Entity> expected{
            Entity{ 4, "5" },
            Entity{ 3, "6" },
            Entity{ 2, "7" },
            Entity{ 1, "8" },
            Entity{ 0, "9" },
        };
        ASSERT_EQ(result, expected);
    }

    //OrderBy mixing ordering column and primitive column.
    {
        auto selecter = EntitySet().BeginSelect();
        auto result = selecter.OrderBy(table.ID.Desc(), table.Name).Execute();
        std::vector<Entity> expected{
            Entity{ 4, "5" },
            Entity{ 3, "6" },
            Entity{ 2, "7" },
            Entity{ 1, "8" },
            Entity{ 0, "9" },
        };
        ASSERT_EQ(result, expected);
    }
}


TEST_F(ORMQueryTest, LimitSelecter) {

    auto& table = Entity::TableType::GetInstance();

    {
        auto result = EntitySet().BeginSelect().Limit(0).Execute();
        ASSERT_EQ(result.size(), 0);
    }

    {
        auto result = EntitySet().BeginSelect().Limit(1).Execute();
        std::vector<Entity> expected{
            Entity{ 0, "9" }
        };
        ASSERT_EQ(result, expected);
    }

    {
        auto result = EntitySet().BeginSelect().Limit(6).Execute();
        std::vector<Entity> expected{
            Entity{ 0, "9" },
            Entity{ 1, "8" },
            Entity{ 2, "7" },
            Entity{ 3, "6" },
            Entity{ 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }
}

}