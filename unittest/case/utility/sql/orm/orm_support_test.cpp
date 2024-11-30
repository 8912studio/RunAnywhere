#include <type_traits>
#include <gtest/gtest.h>
#include "utility/sql/orm/orm_support.h"
#include "utility/sql/orm/primary_key_traits.h"

namespace {

struct Person {
    SQL_ENTITY;
    int ID;
    std::string Name;
    std::optional<int> NullableID;
    std::optional<std::string> NullableName;
};

SQL_TABLE_BEGIN(person, Person)
SQL_COLUMN(id, ID)
SQL_COLUMN(name, Name)
SQL_COLUMN(nullable_id, NullableID)
SQL_COLUMN(nullable_name, NullableName)
SQL_TABLE_END;

TEST(ORMSupportTest, BasicMetaInfo) {

    using PersonTable = Person::TableType;

    //Check types.
    {
        static_assert(!std::is_copy_constructible_v<PersonTable>);
        static_assert(!std::is_copy_assignable_v<PersonTable>);
        static_assert(!std::is_move_constructible_v<PersonTable>);
        static_assert(!std::is_move_assignable_v<PersonTable>);

        static_assert(!std::is_copy_constructible_v<PersonTable::idType>);
        static_assert(!std::is_copy_assignable_v<PersonTable::idType>);
        static_assert(!std::is_move_constructible_v<PersonTable::idType>);
        static_assert(!std::is_move_assignable_v<PersonTable::idType>);

        static_assert(!std::is_copy_constructible_v<PersonTable::nameType>);
        static_assert(!std::is_copy_assignable_v<PersonTable::nameType>);
        static_assert(!std::is_move_constructible_v<PersonTable::nameType>);
        static_assert(!std::is_move_assignable_v<PersonTable::nameType>);
    }

    const auto& table = PersonTable::GetInstance();

    //Check AbstractTable interfaces.
    {
        ASSERT_EQ(table.GetName(), "person");
        ASSERT_EQ(table.GetAbstractPrimaryKey(), nullptr);

        auto columns = table.GetAbstractColumns();
        ASSERT_EQ(columns.size(), 4);
        ASSERT_EQ(columns[0], &table.id);
        ASSERT_EQ(columns[1], &table.name);
        ASSERT_EQ(columns[2], &table.nullable_id);
        ASSERT_EQ(columns[3], &table.nullable_name);
    }

    //Check self interface.
    {
        auto columns = table.GetColumns();
        ASSERT_EQ(columns.size(), 4);
        ASSERT_EQ(columns[0], &table.id);
        ASSERT_EQ(columns[1], &table.name);
        ASSERT_EQ(columns[2], &table.nullable_id);
        ASSERT_EQ(columns[3], &table.nullable_name);
    }

    //Check columns.
    {
        ASSERT_EQ(table.id.GetName(), "id");
        ASSERT_EQ(table.id.GetDataType(), ra::utility::sql::DataType::Integer);
        ASSERT_EQ(table.id.IsNullable(), false);

        ASSERT_EQ(table.name.GetName(), "name");
        ASSERT_EQ(table.name.GetDataType(), ra::utility::sql::DataType::Text);
        ASSERT_EQ(table.name.IsNullable(), false);

        ASSERT_EQ(table.nullable_id.GetName(), "nullable_id");
        ASSERT_EQ(table.nullable_id.GetDataType(), ra::utility::sql::DataType::Integer);
        ASSERT_EQ(table.nullable_id.IsNullable(), true);

        ASSERT_EQ(table.nullable_name.GetName(), "nullable_name");
        ASSERT_EQ(table.nullable_name.GetDataType(), ra::utility::sql::DataType::Text);
        ASSERT_EQ(table.nullable_name.IsNullable(), true);
    }
}


struct PersonPK1 {
    SQL_ENTITY;
    int ID;
    std::string Name;
};

SQL_TABLE_BEGIN(person_pk1, PersonPK1)
SQL_COLUMN(id, ID)
SQL_COLUMN(name, Name)
SQL_PRIMARY_KEY(id)
SQL_TABLE_END;

TEST(ORMSupportTest, SingleColumnPrimaryKey) {

    using PersonTable = PersonPK1::TableType;

    static_assert(!std::is_copy_constructible_v<PersonTable::PrimaryKeyType>);
    static_assert(!std::is_copy_assignable_v<PersonTable::PrimaryKeyType>);
    static_assert(!std::is_move_constructible_v<PersonTable::PrimaryKeyType>);
    static_assert(!std::is_move_assignable_v<PersonTable::PrimaryKeyType>);

    const auto& table = PersonTable::GetInstance();
    auto abstract_primary_key = table.GetAbstractPrimaryKey();
    ASSERT_EQ(abstract_primary_key, &table.PrimaryKey);

    ASSERT_FALSE(table.PrimaryKey.IsAutoincrement());

    auto abstract_columns = table.PrimaryKey.GetAbstractColumns();
    ASSERT_EQ(abstract_columns.size(), 1);
    ASSERT_EQ(abstract_columns[0], &table.id);

    auto columns = table.PrimaryKey.GetColumns();
    ASSERT_EQ(abstract_columns.size(), 1);
    ASSERT_EQ(abstract_columns[0], &table.id);
}


struct PersonPK2 {
    SQL_ENTITY;
    int ID;
    std::string Name;
    int Age;
};

SQL_TABLE_BEGIN(person_pk2, PersonPK2)
SQL_COLUMN(id, ID)
SQL_COLUMN(name, Name)
SQL_COLUMN(age, Age)
SQL_PRIMARY_KEY(id, name)
SQL_TABLE_END;

TEST(ORMSupportTest, MultipleColumnPrimaryKey) {

    using PersonTable = PersonPK2::TableType;

    static_assert(!std::is_copy_constructible_v<PersonTable::PrimaryKeyType>);
    static_assert(!std::is_copy_assignable_v<PersonTable::PrimaryKeyType>);
    static_assert(!std::is_move_constructible_v<PersonTable::PrimaryKeyType>);
    static_assert(!std::is_move_assignable_v<PersonTable::PrimaryKeyType>);

    const auto& table = PersonTable::GetInstance();
    auto abstract_primary_key = table.GetAbstractPrimaryKey();
    ASSERT_EQ(abstract_primary_key, &table.PrimaryKey);

    ASSERT_FALSE(table.PrimaryKey.IsAutoincrement());

    auto abstract_columns = table.PrimaryKey.GetAbstractColumns();
    ASSERT_EQ(abstract_columns.size(), 2);
    ASSERT_EQ(abstract_columns[0], &table.id);
    ASSERT_EQ(abstract_columns[1], &table.name);

    auto columns = table.PrimaryKey.GetColumns();
    ASSERT_EQ(abstract_columns.size(), 2);
    ASSERT_EQ(abstract_columns[0], &table.id);
    ASSERT_EQ(abstract_columns[1], &table.name);
}


struct PersonPK3 {
    SQL_ENTITY;
    int ID;
    std::string Name;
};

SQL_TABLE_BEGIN(person_pk3, PersonPK3);
SQL_COLUMN(id, ID);
SQL_COLUMN(name, Name);
SQL_PRIMARY_KEY_AUTOINCREMENT(id);
SQL_TABLE_END;

TEST(ORMSupportTest, AutoincrementPrimaryKey) {

    using PersonTable = PersonPK3::TableType;

    const auto& table = PersonTable::GetInstance();
    ASSERT_TRUE(table.PrimaryKey.IsAutoincrement());
}

}