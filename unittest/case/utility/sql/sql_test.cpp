#include <gtest/gtest.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/boxing/numeric.h>
#include <zaf/object/boxing/string.h>
#include <zaf/object/object.h>
#include <zaf/object/property_support.h>
#include "utility/sql/table_schema.h"
#include "utility/sql/orm/orm_support.h"
#include "utility/sql/orm/data_set.h"

class Person {
public:
    int id{};
    std::string name{};
    int age{};
};

SQL_TABLE_BEGIN(Person, Person);
SQL_COLUMN(ID, id);
SQL_COLUMN(Name, name);
SQL_COLUMN(Age, age);
SQL_PRIMARY_KEY_AUTOINCREMENT(ID);
SQL_TABLE_END;

TEST(SQLTest, Table) {

    auto database = ra::utility::sql::Database::Open("test.db");

    ra::utility::sql::DataSet<Person> data_set{ database };

    auto persons = data_set.SelectAll();

    auto person = data_set.Select(1);

    Person p;
    p.id = 3;
    p.age = 110;
    p.name = "zzz11";
    data_set.Delete(3);

    auto i = persons.size();
}