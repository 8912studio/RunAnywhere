#include <gtest/gtest.h>
#include <zaf/base/range.h>
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"

using namespace ra::utility::sql;

namespace {

struct Entity {
    SQL_ENTITY;
    int id{};
    std::string name;
    friend auto operator<=>(const Entity&, const Entity&) = default;
};

SQL_TABLE_BEGIN(Entity, Entity)
SQL_COLUMN(id, id)
SQL_COLUMN(name, name)
SQL_PRIMARY_KEY(id)
SQL_TABLE_END

class DataSelectTestFixture : zaf::NonCopyableNonMovable {
public:
    DataSelectTestFixture() {

        auto db_path = "test_data\\data_select_test.db";
        std::filesystem::remove(db_path);

        database_ = Database::Open(db_path);
        data_set_.emplace(*database_);

        for (auto index : zaf::Range(0, 3)) {
            Entity entity;
            entity.id = static_cast<int>(index);
            entity.name = std::to_string(index);
            data_set_->Insert(entity);
        }
    }

    ~DataSelectTestFixture() {
        database_.reset();
    }

    Database& DB() {
        return *database_;
    }

    ra::utility::sql::DataSet<Entity>& DataSet() {
        return *data_set_;
    }

private:
    std::optional<Database> database_;
    std::optional<ra::utility::sql::DataSet<Entity>> data_set_;
};


TEST(ORMTest, SelectAll) {

    DataSelectTestFixture fixture;

    auto result = fixture.DataSet().SelectAll();
    std::vector<Entity> expected{
        { 0, "0" },
        { 1, "1" },
        { 2, "2" },
    };
    ASSERT_EQ(result, expected);
}


TEST(ORMTest, SelectByPrimaryKey) {

    DataSelectTestFixture fixture;

    auto result = fixture.DataSet().Select(1);
    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->id, 1);
    ASSERT_EQ(result->name, "1");

    result = fixture.DataSet().Select(3);
    ASSERT_FALSE(result.has_value());
}

}