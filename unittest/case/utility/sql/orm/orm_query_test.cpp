#include "orm_query_test.h"

using namespace ra::utility::sql;

namespace ra::test {

void ORMQueryTest::SetUp() {

    auto db_path = "test_data\\ORMQueryTest.db";
    std::filesystem::remove(db_path);

    db_ = Database::Open(db_path);
    entity_set_.emplace(*db_);
    entity_pk2_set_.emplace(*db_);

    for (auto index : zaf::Range(0, 5)) {

        Entity entity;
        entity.id = static_cast<int>(index);
        entity.name = std::to_string(9 - index);
        entity_set_->Insert(entity);

        EntityPK2 entity_pk1;
        entity_pk1.id = static_cast<int>(index);
        entity_pk1.name = std::to_string(9 - index);
        entity_pk2_set_->Insert(entity_pk1);
    }
}

}

