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
            entity.name = std::to_string(index);
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

    {
        auto result = fixture.DataSet().BeginSelect().Execute();
        std::vector<Entity> expected{
            Entity{ 0, "0" },
            Entity{ 1, "1" },
            Entity{ 2, "2" },
            Entity{ 3, "3" },
            Entity{ 4, "4" },
        };
        ASSERT_EQ(result, expected);
    }
}


TEST(ORMTest, SelectColumns) {

    SelectQueryTestFixture fixture;

    auto& table = Entity::TableType::GetInstance();

    {
        auto result = fixture.DataSet().BeginSelect(table.ID).Execute();
        std::vector<int> expected{ 0, 1, 2, 3, 4 };
        ASSERT_EQ(result, expected);
    }

    {
        auto result = fixture.DataSet().BeginSelect(table.ID, table.Name).Execute();
        std::vector<std::tuple<int, std::string>> expected{
            { 0, "0" },
            { 1, "1" },
            { 2, "2" },
            { 3, "3" },
            { 4, "4" },
        };
        ASSERT_EQ(result, expected);
    }
}

}