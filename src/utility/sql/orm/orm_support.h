#pragma once

#include <zaf/base/non_copyable.h>
#include "utility/sql/orm/entity_meta.h"
#include "utility/sql/orm/entity_field.h"
#include "utility/sql/orm/entity_key.h"
#include "utility/sql/orm/field_traits.h"

#define SQL_ENTITY_BEGIN(Class) \
template<> \
class sql__EntityMeta<Class> : zaf::NonCopyableNonMovable { \
public: \
    using EntityType = Class; \
    static const sql__EntityMeta& Instance() { \
        static sql__EntityMeta instance; \
        return instance; \
    } \
    static constexpr std::string_view EntityName() { return #Class; } \
    const std::vector<EntityField<EntityType>*>& Fields() const noexcept { \
        return fields_; \
    } \
private: \
    sql__EntityMeta() = default; \
    std::vector<EntityField<EntityType>*> fields_;


#define SQL_ENTITY_FIELD(FieldName, ClassField) \
public: \
    class FieldName##Type : public ra::utility::sql::EntityField<EntityType> { \
    public: \
        using ValueType = decltype(((EntityType*)nullptr)->ClassField); \
        using EntityField::EntityField; \
        std::string_view Name() const override { \
            return #FieldName; \
        } \
        void BindValueToStatement( \
            ra::utility::sql::Statement& statement, \
            int parameter_index, \
            const EntityType& entity) const override { \
            statement.BindParameter(parameter_index, entity.ClassField); \
        } \
        void GetValueFromStatement( \
            const ra::utility::sql::Statement& statement, \
            int column_index, \
            EntityType& entity) const override { \
            entity.ClassField = ra::utility::sql::GetValueFromStatement<ValueType>( \
                statement, \
                column_index); \
        } \
    }; \
    FieldName##Type FieldName{ fields_ };


#define SQL_ENTITY_PRIMARY_KEY(...) \
public:\
    using PrimaryKeyType = decltype(ra::utility::sql::MakeEntityKey<EntityType>(__VA_ARGS__));\
    PrimaryKeyType PrimaryKey = ra::utility::sql::MakeEntityKey<EntityType>(__VA_ARGS__);


#define SQL_ENTITY_END };