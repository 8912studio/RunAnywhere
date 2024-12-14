#include <gtest/gtest.h>
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"
#include "utility/sql/orm/select/primitive_selecter.h"
#include "utility/sql/orm/select/ordering_column.h"

using namespace ra::utility::sql;

namespace {

struct Entity {

    SQL_ENTITY;

    int id{};
    std::string name;

    friend auto operator<=>(const Entity&, const Entity&) = default;
};

SQL_TABLE_BEGIN(Entity, Entity)
SQL_COLUMN(ID, id)
SQL_COLUMN(Name, name)
SQL_INDEX(ID)
SQL_INDEX(ID, Name)
SQL_TABLE_END


class SelectQueryTestFixture : zaf::NonCopyableNonMovable {
public:
    SelectQueryTestFixture() {

        auto db_path = "test_data\\select_query_test.db";
        std::filesystem::remove(db_path);

        database_ = Database::Open(db_path);

        data_set_.emplace(*database_);

        for (auto index : zaf::Range(0, 5)) {
            Entity entity;
            entity.id = static_cast<int>(index);
            entity.name = std::to_string(9 - index);
            data_set_->Insert(entity);
        }
    }

    ~SelectQueryTestFixture() {
        database_.reset();
    }

    Database& DB() {
        return *database_;
    }

    DataSet<Entity>& DataSet() {
        return *data_set_;
    }

private:
    std::optional<Database> database_;
    std::optional<ra::utility::sql::DataSet<Entity>> data_set_;
};


TEST(ORMTest, SelectEntity) {

    SelectQueryTestFixture fixture;
    auto& table = Entity::TableType::GetInstance();

    // Primitive
    {
        auto result = fixture.DataSet().BeginSelect().Execute();
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
        auto selecter = fixture.DataSet().BeginSelect().Where(table.ID == 2 || table.ID == 3);
        auto result = selecter.Execute();
        std::vector<Entity> expected{
            Entity{ 2, "7" },
            Entity{ 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // Where + OrderBy
    {
        auto where_selecter = fixture.DataSet().BeginSelect().Where(
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
        auto where_selecter = fixture.DataSet().BeginSelect().Where(
            table.ID == 1 || table.ID == 3);
        auto result = where_selecter.OrderBy(table.Name).Limit(1).Execute();
        std::vector<Entity> expected{
            Entity{ 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // OrderBy
    {
        auto result = fixture.DataSet().BeginSelect().OrderBy(table.Name).Execute();
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
        auto result = fixture.DataSet().BeginSelect().OrderBy(table.Name).Limit(2).Execute();
        std::vector<Entity> expected{
            Entity{ 4, "5" },
            Entity{ 3, "6" },
        };
        ASSERT_EQ(result, expected);
    }

    // Limit
    {
        auto result = fixture.DataSet().BeginSelect().Limit(3).Execute();
        std::vector<Entity> expected{
            Entity{ 0, "9" },
            Entity{ 1, "8" },
            Entity{ 2, "7" },
        };
        ASSERT_EQ(result, expected);
    }
}


TEST(ORMTest, SelectSingleColumn) {

    SelectQueryTestFixture fixture;
    auto& table = Entity::TableType::GetInstance();
    auto primitive_selecter = fixture.DataSet().BeginSelect(table.ID);

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


TEST(ORMTest, SelectMultipleColumns) {

    SelectQueryTestFixture fixture;
    auto& table = Entity::TableType::GetInstance();
    auto primitive_selecter = fixture.DataSet().BeginSelect(table.ID, table.Name);

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


TEST(ORMTest, WhereSelecter) {

    SelectQueryTestFixture fixture;
    auto& table = Entity::TableType::GetInstance();
    auto primitive_selecter = fixture.DataSet().BeginSelect();

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


TEST(ORMTest, OrderBySelecter) {

    SelectQueryTestFixture fixture;
    auto& table = Entity::TableType::GetInstance();

    //OrderBy using single column.
    {
        auto result = fixture.DataSet().BeginSelect().OrderBy(table.ID).Execute();
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
        auto result = fixture.DataSet().BeginSelect().OrderBy(table.Name, table.ID).Execute();
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
        auto result = fixture.DataSet().BeginSelect().OrderBy(table.Name.Asc()).Execute();
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
        auto result = fixture.DataSet().BeginSelect().OrderBy(table.ID.Desc()).Execute();
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
        auto selecter = fixture.DataSet().BeginSelect();
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


TEST(ORMTest, LimitSelecter) {

    SelectQueryTestFixture fixture;
    auto& table = Entity::TableType::GetInstance();

    {
        auto result = fixture.DataSet().BeginSelect().Limit(0).Execute();
        ASSERT_EQ(result.size(), 0);
    }

    {
        auto result = fixture.DataSet().BeginSelect().Limit(1).Execute();
        std::vector<Entity> expected{
            Entity{ 0, "9" }
        };
        ASSERT_EQ(result, expected);
    }

    {
        auto result = fixture.DataSet().BeginSelect().Limit(6).Execute();
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