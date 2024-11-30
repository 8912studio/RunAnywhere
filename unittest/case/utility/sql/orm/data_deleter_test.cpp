#include <gtest/gtest.h>
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"

using namespace ra::utility::sql;

namespace {

class DataDeleterTestFixture : zaf::NonCopyableNonMovable {
public:
    DataDeleterTestFixture() {

        auto db_path = "test_data\\data_deleter_test.db";
        std::filesystem::remove(db_path);

        database_ = Database::Open(db_path);
    }

    ~DataDeleterTestFixture() {
        database_.reset();
    }

    Database& DB() {
        return *database_;
    }

private:
    std::optional<Database> database_;
};


struct EntityPK1 {
    SQL_ENTITY;
    int id{};
    std::string name;
};

SQL_TABLE_BEGIN(EntityPK1, EntityPK1)
SQL_COLUMN(id, id)
SQL_COLUMN(name, name)
SQL_PRIMARY_KEY(id)
SQL_TABLE_END

TEST(DataDeleterTest, SingleColumnPrimaryKey) {

    DataDeleterTestFixture fixture;
    DataSet<EntityPK1> data_set{ fixture.DB() };

    EntityPK1 entity;
    entity.id = 1;
    entity.name = "first";
    data_set.Insert(entity);

    entity.id = 2;
    entity.name = "second";
    data_set.Insert(entity);

    data_set.Delete(1);
    auto statement = fixture.DB().PrepareStatement("select * from EntityPK1");
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 2);
    ASSERT_EQ(statement.GetColumnText(1), "second");

    data_set.Delete(2);
    statement = fixture.DB().PrepareStatement("select * from EntityPK1");
    ASSERT_FALSE(statement.Step());
}


struct EntityPK2 {
    SQL_ENTITY;
    int id{};
    std::string name;
    int age{};
};

SQL_TABLE_BEGIN(EntityPK2, EntityPK2)
SQL_COLUMN(id, id)
SQL_COLUMN(name, name)
SQL_COLUMN(age, age)
SQL_PRIMARY_KEY(id, name)
SQL_TABLE_END

TEST(DataDeleterTest, MultiColumnPrimaryKey) {

    DataDeleterTestFixture fixture;
    DataSet<EntityPK2> data_set{ fixture.DB() };

    EntityPK2 entity;
    entity.id = 1;
    entity.name = "first";
    entity.age = 101;
    data_set.Insert(entity);

    entity.id = 2;
    entity.name = "second";
    entity.age = 201;
    data_set.Insert(entity);

    data_set.Delete(std::tuple<int, std::string>{ 1, "first" });
    auto statement = fixture.DB().PrepareStatement("select * from EntityPK2");
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 2);
    ASSERT_EQ(statement.GetColumnText(1), "second");
    ASSERT_EQ(statement.GetColumnInt(2), 201);

    data_set.Delete(std::tuple<int, std::string>{ 2, "second" });
    statement = fixture.DB().PrepareStatement("select * from EntityPK2");
    ASSERT_FALSE(statement.Step());
}

}