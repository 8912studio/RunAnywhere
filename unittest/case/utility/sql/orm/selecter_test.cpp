#include <gtest/gtest.h>
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"
#include "utility/sql/orm/select/primitive_selecter.h"

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
}


TEST(ORMTest, SelectMultipleColumns) {

    SelectQueryTestFixture fixture;
    auto& table = Entity::TableType::GetInstance();

    {
        auto result = fixture.DataSet().BeginSelect(table.ID, table.Name).Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 0, "9" },
            { 1, "8" },
            { 2, "7" },
            { 3, "6" },
            { 4, "5" },
        };
        ASSERT_EQ(result, expected);
    }
}

}