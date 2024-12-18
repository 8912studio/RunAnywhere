#pragma once

#include <gtest/gtest.h>
#include "utility/sql/database.h"
#include "utility/sql/orm/data_set.h"
#include "utility/sql/orm/orm_support.h"

namespace ra::test {

struct Entity {
    SQL_ENTITY;
    int id{};
    std::string name;
    friend auto operator<=>(const Entity&, const Entity&) = default;
};
SQL_TABLE_BEGIN(Entity, Entity)
SQL_COLUMN(ID, id)
SQL_COLUMN(Name, name)
SQL_PRIMARY_KEY(ID)
SQL_INDEX(ID)
SQL_INDEX(ID, Name)
SQL_TABLE_END


struct EntityPK2 {
    SQL_ENTITY;
    int id{};
    std::string name;
    friend auto operator<=>(const EntityPK2&, const EntityPK2&) = default;
};

SQL_TABLE_BEGIN(EntityPK2, EntityPK2)
SQL_COLUMN(id, id)
SQL_COLUMN(name, name)
SQL_PRIMARY_KEY(id, name)
SQL_TABLE_END


class ORMQueryTest : public testing::Test {
protected:
    void SetUp() override;

    ra::utility::sql::DataSet<Entity>& EntitySet() {
        return *entity_set_;
    }

    ra::utility::sql::DataSet<EntityPK2>& EntityPK2Set() {
        return *entity_pk2_set_;
    }

private:
    std::optional<ra::utility::sql::Database> db_;
    std::optional<ra::utility::sql::DataSet<Entity>> entity_set_;
    std::optional<ra::utility::sql::DataSet<EntityPK2>> entity_pk2_set_;
};

}