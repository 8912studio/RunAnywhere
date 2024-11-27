#include <gtest/gtest.h>
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"

using namespace ra::utility::sql;

namespace {

class DataSelectorTestFixture : zaf::NonCopyableNonMovable {
public:
    DataSelectorTestFixture() {

        auto db_path = "test_data\\data_selector_test.db";
        std::filesystem::remove(db_path);

        database_ = Database::Open(db_path);
    }

    ~DataSelectorTestFixture() {
        database_.reset();
    }

    Database& DB() {
        return *database_;
    }

private:
    std::optional<Database> database_;
};

}


struct Entity {
    int id{};
    std::string name;
};

SQL_TABLE_BEGIN(Entity, Entity);
SQL_COLUMN(id, id);
SQL_COLUMN(name, name);
SQL_TABLE_END;

TEST(DataSelectorTest, SelectAll) {

    DataSelectorTestFixture fixture;
    DataSet<Entity> data_set{ fixture.DB() };

    Entity entity;
    entity.id = 11;
    entity.name = "abc";
    data_set.Insert(entity);

    entity.id = 22;
    entity.name = "k32";
    data_set.Insert(entity);

    auto all = data_set.SelectAll();
    ASSERT_EQ(all.size(), 2);
    ASSERT_EQ(all[0].id, 11);
    ASSERT_EQ(all[0].name, "abc");
    ASSERT_EQ(all[1].id, 22);
    ASSERT_EQ(all[1].name, "k32");
}