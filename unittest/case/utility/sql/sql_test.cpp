#include <gtest/gtest.h>
#include <zaf/base/string/encoding_conversion.h>
#include <zaf/object/boxing/numeric.h>
#include <zaf/object/boxing/string.h>
#include <zaf/object/object.h>
#include <zaf/object/property_support.h>
#include "utility/sql/table_definition.h"

using namespace ra::utility::sql;

class Person : public zaf::Object {
public:
    ZAF_OBJECT;

    int Age() const {
        return age_;
    }

    void SetAge(int age) {
        age_ = age;
    }

    const std::string& Name() const {
        return name_;
    }

    void SetName(std::string name) {
        name_ = std::move(name);
    }

private:
    int age_{};
    std::string name_;
};

ZAF_OBJECT_BEGIN(Person);
ZAF_OBJECT_PROPERTY(Age);
ZAF_OBJECT_PROPERTY(Name);
ZAF_OBJECT_END;

ZAF_OBJECT_IMPL(Person);


ColumnDefinition CreateColumnDefinition(zaf::ObjectProperty* property) {

    ColumnDefinition result;
    result.name = zaf::ToUTF8String(property->Name());

    auto value_type = property->ValueType();
    if (value_type == zaf::Int32::StaticType() ||
        value_type == zaf::Int64::StaticType()) {
        result.data_type = DataType::Integer;
    }
    else if (value_type == zaf::String::StaticType()) {
        result.data_type = DataType::Text;
    }

    return result;
}


TableDefinition CreateTableDefinition(
    zaf::ObjectType* type,  
    const std::vector<zaf::ObjectProperty*>& properties) {

    TableDefinition result;
    result.name = zaf::ToUTF8String(type->Name());
    for (auto each_property : properties) {
        result.columns.push_back(CreateColumnDefinition(each_property));
    }
    return result;
}


TEST(SQLTest, TableDefinition) {

    auto person_type = Person::Type::Instance();

    TableDefinition table_definition{
        .name = zaf::ToUTF8String(person_type->Name()),
        .columns = {
            {
                .name = zaf::ToUTF8String(person_type->AgeProperty->Name()),
                .data_type = DataType::Integer,
            },
            {
                .name = zaf::ToUTF8String(person_type->NameProperty->Name()),
                .data_type = DataType::Text,
            },
        },
    };
}