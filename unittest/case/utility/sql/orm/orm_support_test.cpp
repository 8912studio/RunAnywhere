#include <type_traits>
#include <gtest/gtest.h>
#include "utility/sql/orm/orm_support.h"

struct Person {
    int ID;
    int Age;
    std::string Name;
};

SQL_TABLE_BEGIN(person, Person);
SQL_COLUMN(id, ID);
SQL_COLUMN(name, Name);
SQL_TABLE_END;

TEST(ORMSupportTest, BasicMetaInfo) {

    using PersonTable = ra::utility::sql::Table<Person>;

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
        ASSERT_EQ(columns.size(), 2);
        ASSERT_EQ(columns[0], &table.id);
        ASSERT_EQ(columns[1], &table.name);
    }

    //Check self interface.
    {
        auto columns = table.GetColumns();
        ASSERT_EQ(columns.size(), 2);
        ASSERT_EQ(columns[0], &table.id);
        ASSERT_EQ(columns[1], &table.name);
    }

    //Check columns.
    {
        ASSERT_EQ(table.id.GetName(), "id");
        ASSERT_EQ(table.id.GetDataType(), ra::utility::sql::DataType::Integer);

        ASSERT_EQ(table.name.GetName(), "name");
        ASSERT_EQ(table.name.GetDataType(), ra::utility::sql::DataType::Text);
    }
}


struct PersonPK1{
    int ID;
    std::string Name;
};

SQL_TABLE_BEGIN(person, PersonPK1);
SQL_COLUMN(id, ID);
SQL_COLUMN(name, Name);
SQL_PRIMARY_KEY(id);
SQL_TABLE_END;

TEST(ORMSupportTest, SingleColumnPrimaryKey) {

    using PersonTable = ra::utility::sql::Table<PersonPK1>;

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
    int ID;
    std::string Name;
    int Age;
};

SQL_TABLE_BEGIN(person, PersonPK2);
SQL_COLUMN(id, ID);
SQL_COLUMN(name, Name);
SQL_COLUMN(age, Age);
SQL_PRIMARY_KEY(id, name);
SQL_TABLE_END;

TEST(ORMSupportTest, MultipleColumnPrimaryKey) {

    using PersonTable = ra::utility::sql::Table<PersonPK2>;

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
    int ID;
    std::string Name;
};

SQL_TABLE_BEGIN(person, PersonPK3);
SQL_COLUMN(id, ID);
SQL_COLUMN(name, Name);
SQL_PRIMARY_KEY_AUTOINCREMENT(id);
SQL_TABLE_END;

TEST(ORMSupportTest, AutoincrementPrimaryKey) {

    using PersonTable = ra::utility::sql::Table<PersonPK3>;

    const auto& table = PersonTable::GetInstance();
    ASSERT_TRUE(table.PrimaryKey.IsAutoincrement());
}