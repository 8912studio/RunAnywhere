#include <gtest/gtest.h>
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"

using namespace ra::utility::sql;

namespace {

class DataUpdaterTestFixture : zaf::NonCopyableNonMovable {
public:
    DataUpdaterTestFixture() {

        auto db_path = "test_data\\data_updater_test.db";
        std::filesystem::remove(db_path);

        database_ = Database::Open(db_path);
    }

    ~DataUpdaterTestFixture() {
        database_.reset();
    }

    Database& DB() {
        return *database_;
    }

private:
    std::optional<Database> database_;
};


struct EntityPK1 {
    int id{};
    std::string name;
};

SQL_TABLE_BEGIN(EntityPK1, EntityPK1);
SQL_COLUMN(id, id);
SQL_COLUMN(name, name);
SQL_PRIMARY_KEY(id);
SQL_TABLE_END;

TEST(DataUpdaterTest, SingleColumnPrimaryKey) {

    DataUpdaterTestFixture fixture;
    DataSet<EntityPK1> data_set{ fixture.DB() };

    EntityPK1 entity;
    entity.id = 1;
    entity.name = "100";
    data_set.Insert(entity);

    entity.id = 2;
    entity.name = "200";
    data_set.Insert(entity);

    entity.id = 1;
    entity.name = "300";
    data_set.Update(entity);

    auto statement = fixture.DB().PrepareStatement("select * from EntityPK1");
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 1);
    ASSERT_EQ(statement.GetColumnText(1), "300");
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 2);
    ASSERT_EQ(statement.GetColumnText(1), "200");
}



struct EntityPK2 {
    int id{};
    std::string name;
    int age{};
};

SQL_TABLE_BEGIN(EntityPK2, EntityPK2);
SQL_COLUMN(id, id);
SQL_COLUMN(name, name);
SQL_COLUMN(age, age);
SQL_PRIMARY_KEY(id, name);
SQL_TABLE_END;

TEST(DataUpdaterTest, MultipleColumnPrimaryKey) {

    DataUpdaterTestFixture fixture;
    DataSet<EntityPK2> data_set{ fixture.DB() };

    EntityPK2 entity;
    entity.id = 1;
    entity.name = "100";
    entity.age = 45;
    data_set.Insert(entity);

    entity.id = 2;
    entity.name = "200";
    entity.age = 13;
    data_set.Insert(entity);

    entity.id = 1;
    entity.name = "100";
    entity.age = 98;
    data_set.Update(entity);

    auto statement = fixture.DB().PrepareStatement("select * from EntityPK2");
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 1);
    ASSERT_EQ(statement.GetColumnText(1), "100");
    ASSERT_EQ(statement.GetColumnInt(2), 98);
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 2);
    ASSERT_EQ(statement.GetColumnText(1), "200");
}

}