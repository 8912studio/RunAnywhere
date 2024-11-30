#include <gtest/gtest.h>
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"

using namespace ra::utility::sql;

namespace {

class DataInserterTestFixture : zaf::NonCopyableNonMovable {
public:
    DataInserterTestFixture() {

        auto db_path = "test_data\\data_inserter_test.db";
        std::filesystem::remove(db_path);

        database_ = Database::Open(db_path);
    }

    ~DataInserterTestFixture() {
        database_.reset();
    }

    Database& DB() {
        return *database_;
    }

private:
    std::optional<Database> database_;
};


struct EntityNoPK {
    SQL_ENTITY;
    int integer{};
    std::string string;
    std::optional<int> null_integer1;
    std::optional<int> null_integer2;
    std::optional<std::string> null_string1;
    std::optional<std::string> null_string2;
};

SQL_TABLE_BEGIN(Entity, EntityNoPK)
SQL_COLUMN(IntValue, integer)
SQL_COLUMN(StrValue, string)
SQL_COLUMN(NullInt1, null_integer1)
SQL_COLUMN(NullInt2, null_integer2)
SQL_COLUMN(NullStr1, null_string1)
SQL_COLUMN(NullStr2, null_string2)
SQL_TABLE_END

TEST(DataInserterTest, NoPrimaryKey) {

    DataInserterTestFixture fixture;
    DataSet<EntityNoPK> data_set{ fixture.DB() };

    EntityNoPK entity;
    entity.integer = 100;
    entity.string = "aaa";
    entity.null_integer1 = 101;
    entity.null_string1 = "adc";
    data_set.Insert(entity);

    auto statement = fixture.DB().PrepareStatement(std::format("select * from Entity;"));
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 100);             //IntValue
    ASSERT_EQ(statement.GetColumnText(1), "aaa");          //StrValue
    ASSERT_EQ(statement.GetColumnInt(2), 101);             //NullInt1
    ASSERT_EQ(statement.GetColumnType(3), DataType::Null); //NullInt2
    ASSERT_EQ(statement.GetColumnText(4), "adc");          //NullStr1
    ASSERT_EQ(statement.GetColumnType(5), DataType::Null); //NullStr2
}


struct EntityPK1 {
    SQL_ENTITY;
    int integer{};
    std::string string;
};

SQL_TABLE_BEGIN(EntityPK1, EntityPK1)
SQL_COLUMN(IntValue, integer)
SQL_COLUMN(StrValue, string)
SQL_PRIMARY_KEY(IntValue)
SQL_TABLE_END

TEST(DataInserterTest, PrimaryKeySingleColumn) {

    DataInserterTestFixture fixture;
    DataSet<EntityPK1> data_set{ fixture.DB() };

    EntityPK1 entity;
    entity.integer = 101;
    entity.string = "aad";
    data_set.Insert(entity);

    auto statement = fixture.DB().PrepareStatement(std::format("select * from EntityPK1;"));
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 101);
    ASSERT_EQ(statement.GetColumnText(1), "aad");
}


struct EntityPK2 {
    SQL_ENTITY;
    int integer{};
    std::string string;
    int integer2{};
};

SQL_TABLE_BEGIN(EntityPK2, EntityPK2)
SQL_COLUMN(IntValue, integer)
SQL_COLUMN(StrValue, string)
SQL_COLUMN(IntValue2, integer2)
SQL_PRIMARY_KEY(IntValue, StrValue)
SQL_TABLE_END

TEST(DataInserterTest, PrimaryKeyMultipleColumn) {

    DataInserterTestFixture fixture;
    DataSet<EntityPK2> data_set{ fixture.DB() };

    EntityPK2 entity;
    entity.integer = 202;
    entity.string = "232";
    entity.integer2 = 404;
    data_set.Insert(entity);

    auto statement = fixture.DB().PrepareStatement(std::format("select * from EntityPK2;"));
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 202);
    ASSERT_EQ(statement.GetColumnText(1), "232");
    ASSERT_EQ(statement.GetColumnInt(2), 404);
}


struct EntityAutoInc {
    SQL_ENTITY;
    int id{};
    std::string name;
};

SQL_TABLE_BEGIN(EntityAutoInc, EntityAutoInc)
SQL_COLUMN(ID, id)
SQL_COLUMN(Name, name)
SQL_PRIMARY_KEY_AUTOINCREMENT(ID)
SQL_TABLE_END

TEST(DataInserterTest, Autoincrement) {

    DataInserterTestFixture fixture;
    DataSet<EntityAutoInc> data_set{ fixture.DB() };

    EntityAutoInc entity;
    entity.name = "first";
    int id1 = data_set.InsertWithAutoincrement(entity);
    ASSERT_EQ(id1, 1);

    entity.name = "second";
    int id2 = data_set.InsertWithAutoincrement(entity);
    ASSERT_EQ(id2, 2);

    auto statement = fixture.DB().PrepareStatement(std::format("select * from EntityAutoInc;"));
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 1);
    ASSERT_EQ(statement.GetColumnText(1), "first");
    ASSERT_TRUE(statement.Step());
    ASSERT_EQ(statement.GetColumnInt(0), 2);
    ASSERT_EQ(statement.GetColumnText(1), "second");
}

}