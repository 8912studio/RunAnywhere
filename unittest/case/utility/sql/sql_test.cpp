#include <gtest/gtest.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/boxing/numeric.h>
#include <zaf/object/boxing/string.h>
#include <zaf/object/object.h>
#include <zaf/object/property_support.h>
#include "utility/sql/composite_key.h"
#include "utility/sql/table_schema.h"
#include "utility/sql/table.h"
#include "utility/sql/orm/orm_support.h"

using namespace ra::utility::sql;

class Person {
public:
    int id{};
    std::string name{};
    int age{};
};

SQL_ENTITY_BEGIN(Person);
SQL_ENTITY_FIELD(ID, id);
SQL_ENTITY_FIELD(Name, name);
SQL_ENTITY_PRIMARY_KEY(ID);
SQL_ENTITY_END;

template<>
class Entity<Person> : zaf::NonCopyableNonMovable {
public:
    static Entity& Instance() {
        static Entity instance;
        return instance;
    }

public:
    virtual std::string EntityName() const {
        return "Person";
    }

    const std::vector<Property<Person>*>& Properties() const noexcept {
        return properties_;
    }

private:
    Entity() = default;

private:
    std::vector<Property<Person>*> properties_;

public:
    class IDProperty : public Property<Person> {
    public:
        using ValueType = decltype(((Person*)nullptr)->id);
        using Property::Property;
        std::string Name() const override {
            return "ID";
        }
        void BindValueToStatement(Statement& statement, int column_index, const Person& object) override {
            statement.BindParameter(column_index, object.id);
        }
        void GetValueFromStatement(const Statement& statement, int index, Person& object) override {
            object.id = statement.GetColumnInt(index);
        }
    };
    IDProperty ID{ properties_ };

    class AgeProperty : public Property<Person> {
    public:
        using ValueType = int;
        using Property::Property;
        std::string Name() const override {
            return "Age";
        }
        void BindValueToStatement(Statement& statement, int column_index, const Person& object) override {
            statement.BindParameter(column_index, object.age);
        }
        void GetValueFromStatement(const Statement& statement, int index, Person& object) override {
            object.age = statement.GetColumnInt(index);
        }
    };
    AgeProperty Age{ properties_ };

    class NameProperty : public Property<Person> {
    public:
        using ValueType = std::string;
        using Property::Property;
        std::string Name() const override {
            return "Name";
        }
        void BindValueToStatement(Statement& statement, int column_index, const Person& object) override {
            statement.BindParameter(column_index, object.name);
        }
        void GetValueFromStatement(const Statement& statement, int index, Person& object) override {
            object.name = statement.GetColumnText(index);
        }
    };
    NameProperty Name{ properties_ };

public:
    using PrimaryKeyType = CompositeKey<Person, Entity<Person>::IDProperty>;
    PrimaryKeyType PrimaryKey{
        ID,
    };
};


TEST(SQLTest, Table) {

    auto database = Database::Open("test.db");

    TableSchema table_schema;
    table_schema.name = "Person";
    table_schema.columns = {
        ColumnSchema{ 
            .name = "ID",
            .data_type = DataType::Integer
        },
        ColumnSchema{
            .name = "Age",
            .data_type = DataType::Integer
        },
        ColumnSchema{
            .name = "Name",
            .data_type = DataType::Text
        }
    };
    database.CreateTable(table_schema);

    Table<Person> table{ database };

    Person person;
    person.id = 2;
    person.age = 20;
    person.name = "zplutor";
    table.Insert(person);

    table.Select(1);
}