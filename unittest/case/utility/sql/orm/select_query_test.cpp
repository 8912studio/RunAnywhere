#include <gtest/gtest.h>
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"
#include "utility/sql/orm/select_query.h"

using namespace ra::utility::sql;

namespace {

class SelectQueryTestFixture : zaf::NonCopyableNonMovable {
public:
    SelectQueryTestFixture() {

        auto db_path = "test_data\\select_query_test.db";
        std::filesystem::remove(db_path);

        database_ = Database::Open(db_path);
    }

    ~SelectQueryTestFixture() {
        database_.reset();
    }

    Database& DB() {
        return *database_;
    }

private:
    std::optional<Database> database_;
};


struct Entity {
    SQL_ENTITY;
    int id{};
    std::string name;
};

SQL_TABLE_BEGIN(Entity, Entity)
SQL_COLUMN(ID, id)
SQL_COLUMN(Name, name)
SQL_TABLE_END

TEST(SelectQueryTest, Test) {

    SelectQueryTestFixture fixture;
    DataSet<Entity> data_set{ fixture.DB() };

    for (auto index : zaf::Range(0, 5)) {
        Entity entity;
        entity.id = static_cast<int>(index);
        entity.name = std::to_string(index);
        data_set.Insert(entity);
    }
    
    using TableType = Entity::TableType;
    auto& table = TableType::GetInstance();

    SelectQuery<Entity, TableType::IDType, TableType::NameType> query{ 
        fixture.DB(),
        table.ID,
        table.Name
    };

    auto result = query.Execute();

    result = query.Limit(2).Execute();

    result = query.OrderBy(table.Name).Execute();
}

}